/*
 * Copyright (c) 2023, BlackBerry Limited.
 * Copyright (c) 2023, Renesas Electronics Corporation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#if defined(VARIANT_ipmmu)
#include <hw/rcar_ipmmu-api.h>
#include "ravb.h"

#define ALIGNED_PHYS(paddr)   ((paddr) & 0xfffffffffffff000UL)

#if 0
/* Find the node having paddr within the binary tree */
static iommu_node_t *
iommu_find_node(iommu_node_t *root, uint64_t paddr)
{
    if ((root == NULL) || (paddr == root->paddr)) {
        return root; /* found */

    } else if (paddr > root->paddr) {
        return iommu_find_node(root->right_node, paddr);

    } else { /* (paddr < root->paddr) */
        return iommu_find_node(root->left_node, paddr);
    }
}
#endif

/* Create a new node and map the paddr (size) to generate the translated addr (iova) */
static iommu_node_t *
iommu_new_node(ravb_dev_t *ravb, uint64_t paddr, uint32_t size)
{
    iommu_node_t *node;
    int ret;

    if (ravb->ipmmu_node_count >= IOMMU_NODE_MAX) {
        slogf(_SLOGC_NETWORK, _SLOG_ERROR,
                "ravb: %s() ipmmu_node_count reached max %d. "
                "unable to map paddr 0x%"PRIx64" size %d",
                __func__, ravb->ipmmu_node_count, paddr, size);
        return NULL;
    }

    node = calloc(1, sizeof(*node));
    if (node == NULL) {
        slogf(_SLOGC_NETWORK, _SLOG_ERROR,
                "ravb: %s() calloc failed paddr 0x%"PRIx64" size %d",
                __func__, paddr, size);
        return NULL;
    }

    ret = ipmmu_map(ravb->ipmmu_handle, paddr, size, &node->iova);
    if (ret != EOK) {
        (free)(node);
        return NULL;
    }

    node->paddr = paddr;
    node->size = size;
    node->right_node = NULL;
    node->left_node = NULL;

    ravb->ipmmu_node_count++;

    /* slogf(_SLOGC_NETWORK, _SLOG_ERROR,
                "ravb: new paddr 0x%"PRIx64" size %d iova 0x%x",
                paddr, size, node->iova); */

    return node;
}

/* Find the left-most node (minimum value) within the binary tree */
static iommu_node_t *
iommu_find_min_node(iommu_node_t *root)
{
    if (root == NULL) {
        return NULL;

    } else if (root->left_node != NULL) {
        return iommu_find_min_node(root->left_node);

    }
    return root;
}

/* Delete node having paddr from binary tree */
static iommu_node_t *
iommu_delete_node(ravb_dev_t *ravb, iommu_node_t *root, uint64_t paddr, int unmap)
{
    iommu_node_t *node;

    if (root == NULL) {
        return root;
    }

    if (paddr > root->paddr) {
        root->right_node = iommu_delete_node(ravb, root->right_node, paddr, 1);

    } else if (paddr < root->paddr) {
        root->left_node = iommu_delete_node(ravb, root->left_node, paddr, 1);

    } else {
        if ((root->left_node == NULL) && (root->right_node == NULL)) {
            /* no child */
            if (unmap) {
                ipmmu_unmap(ravb->ipmmu_handle, root->paddr, root->iova, root->size);
            }
            ravb->ipmmu_node_count--;
            (free)(root);
            return NULL;

        } else if ((root->left_node == NULL) || (root->right_node == NULL)) {
            /* single child */
            if (root->left_node == NULL) {
                node = root->right_node;
            } else {
                node = root->left_node;
            }
            if (unmap) {
                ipmmu_unmap(ravb->ipmmu_handle, root->paddr, root->iova, root->size);
            }
            ravb->ipmmu_node_count--;
            (free)(root);
            return node;

        } else {
            /* two children: move-up right child, left child as-is */
            node = iommu_find_min_node(root->right_node);
            /* unmap before overwrite */
            ipmmu_unmap(ravb->ipmmu_handle, root->paddr, root->iova, root->size);
            root->paddr = node->paddr;
            root->size = node->size;
            root->iova = node->iova;
            /* delete the redundant node; no need to unmap */
            root->right_node = iommu_delete_node(ravb, root->right_node, node->paddr, 0);
        }
    }
    return root;
}

/* Free allocated memory on each node within the binary tree
 * NOTE: The actual nodes are not being removed in this function.
 */
static void
iommu_delete_tree(ravb_dev_t *ravb, iommu_node_t *root)
{
    if (root == NULL) {
        return;
    }

    iommu_delete_tree(ravb, root->left_node);
    iommu_delete_tree(ravb, root->right_node);

    ipmmu_unmap(ravb->ipmmu_handle, root->paddr, root->iova, root->size);

    ravb->ipmmu_node_count--;

    /* slogf(_SLOGC_NETWORK, _SLOG_ERROR,
                "ravb: %s() delete paddr 0x%lx, size %d, node_count %d.",
                __func__, root->paddr, root->size, ravb->ipmmu_node_count); */

    (free)(root);
}

/* Get node having paddr (size) */
iommu_node_t *
iommu_get_node(ravb_dev_t *ravb, uint64_t paddr, uint32_t size)
{
    iommu_node_t *new, *cur, *prev;
    uint64_t aligned_paddr_s = ALIGNED_PHYS(paddr);
    uint32_t aligned_size = ALIGN(size, 4096);
    uint64_t aligned_paddr_e = aligned_paddr_s + aligned_size - 1;

    if (ravb->ipmmu_tree_root == NULL) {
        /* first entry */
        ravb->ipmmu_tree_root = iommu_new_node(ravb, aligned_paddr_s, aligned_size);
        return ravb->ipmmu_tree_root;
    }

    cur = ravb->ipmmu_tree_root;
    prev = NULL;
    /* search through the tree */
    while (cur != NULL) {
        prev = cur; /* save previous node */

        if ((aligned_paddr_s == cur->paddr) && (aligned_size == cur->size)) {
            /* reuse this node as-is */

            /* slogf(_SLOGC_NETWORK, _SLOG_ERROR,
                    "ravb: reuse paddr 0x%"PRIx64" size %d",
                    aligned_paddr_s, aligned_size); */

            return (cur);

        } else if ((aligned_paddr_s == cur->paddr) && (aligned_size != cur->size)) {
            /* we can reuse this node but we need to unmap-remap */
            ipmmu_unmap(ravb->ipmmu_handle, cur->paddr, cur->iova, cur->size);

            int ret = ipmmu_map(ravb->ipmmu_handle, cur->paddr, cur->size, &cur->iova);
            if (ret != EOK) {
                /* remove possibly stale node */
                iommu_delete_node(ravb, ravb->ipmmu_tree_root, paddr, 1);
                return NULL;
            }

            /* slogf(_SLOGC_NETWORK, _SLOG_ERROR,
                    "ravb: remap paddr 0x%"PRIx64" size %d",
                    aligned_paddr_s, aligned_size); */

            return (cur);

        /* check for possible overlaps */
        } else if ((aligned_paddr_s > cur->paddr &&
                    aligned_paddr_s <= (cur->paddr + cur->size - 1)) ||
                   (aligned_paddr_e > cur->paddr &&
                    aligned_paddr_e <= (cur->paddr + cur->size - 1))) {

            slogf(_SLOGC_NETWORK, _SLOG_ERROR,
                    "ravb: %s() overlap paddr 0x%"PRIx64" size %d "
                    "[node paddr 0x%lx size %d]",
                    __func__, aligned_paddr_s, aligned_size,
                    cur->paddr, cur->size);

            return NULL;

        } else if (aligned_paddr_s > cur->paddr) {
            cur = cur->right_node;

        } else if (aligned_paddr_s < cur->paddr) {
            cur = cur->left_node;

        }
    }

    /* create new node, add to the tree */
    new = iommu_new_node(ravb, aligned_paddr_s, aligned_size);
    if (aligned_paddr_s > prev->paddr) {
        prev->right_node = new;

    } else {
        prev->left_node = new;

    }

    /* slogf(_SLOGC_NETWORK, _SLOG_ERROR,
            "ravb: new paddr 0x%"PRIx64" size %d",
            aligned_paddr_s, aligned_size); */

    return (new);
}

void
iommu_cleanup(ravb_dev_t *ravb)
{
    if (ravb->ipmmu_tree_root == NULL) {
        return; /* nothing to do */
    }

    iommu_delete_tree(ravb, ravb->ipmmu_tree_root);

    if (ravb->ipmmu_node_count > 0) {
        slogf(_SLOGC_NETWORK, _SLOG_ERROR,
                "ravb: %s() ipmmu_node_count %d is not 0. Potential leak.",
                __func__, ravb->ipmmu_node_count);
    }

    ravb->ipmmu_node_count = 0;
    ravb->ipmmu_tree_root = NULL;
    ipmmu_close(ravb->ipmmu_handle);
}

#endif /* defined(VARIANT_ipmmu) */

