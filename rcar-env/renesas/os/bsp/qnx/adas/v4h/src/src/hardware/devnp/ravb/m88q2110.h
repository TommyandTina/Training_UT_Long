/*
 * Copyright (c) 2023, BlackBerry Limited.
 * Copyright (c) 2022, Renesas Electronics Corporation.
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

#ifndef M88Q2110_H
#define M88Q2110_H

#define MV_PCS_DEV                          3

#ifndef MARVELLX
#define MARVELLX                            0xac2
#endif

#define GIGE_T1_CONTROL_REG                 0x0900
#define GIGE_T1_CONTROL_REG_RESET           (1 << 15)
#define GIGE_T1_CONTROL_REG_LOW_PWR         (1 << 11)


#define GIGE_T1_STATUS_REG                  0x0901
#define GIGE_T1_STATUS_REG_LINK_STATUS      (1 << 2)

#define AUTONEG_STATUS_REG                  0x8001
#define AUTONEG_STATUS_REG_LOCAL_RX_STATUS  (1 << 13)
#define AUTONEG_STATUS_REG_REMOTE_RX_STATUS (1 << 12)
#define AUTONEG_STATUS_BOTH_RX_STATUS  \
    (AUTONEG_STATUS_REG_LOCAL_RX_STATUS | AUTONEG_STATUS_REG_REMOTE_RX_STATUS)

#define PMA_PMD_CTRL                        0x834

typedef enum {
    M88Q2110_A0 = 0x1,
    M88Q2110_A1 = 0x2,
    M88Q2110_A2 = 0x3,
} m88q2110_rev_t ;

#define MRVL_88Q2112_OP_MODE_LEGACY         0x06B0
#define MRVL_88Q2112_OP_MODE_DEFAULT        0x0000

void ravb_mv_phy_state(ravb_dev_t *ravb);
int ravb_mv_phy_init (ravb_dev_t *ravb);
int ravb_is_mv_phy(ravb_dev_t *ravb);
void ravb_mv_phy_fini(ravb_dev_t *ravb);

#endif /* M88Q2110_H */
