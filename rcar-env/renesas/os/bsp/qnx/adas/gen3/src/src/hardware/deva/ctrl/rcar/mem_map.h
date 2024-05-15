#include <sys/neutrino.h>
#if _NTO_VERSION >= 710
#define ADO_DEVICE_MMAP(card, phys_addr, size) ado_device_mmap(card, phys_addr, size)
#define ADO_DEVICE_MUNMAP(card, phys_addr, size) ado_device_munmap(card, phys_addr, size)
#define ADO_PCM_BUF_MAP(card, config, phys_addr, size, flags) ado_pcm_buf_map(card, config, phys_addr, size, flags)
#define ADO_PCM_BUF_ALLOC(card, config, size, flags) ado_pcm_buf_alloc(card, config, size, flags)
#define ADO_PCM_BUF_FREE(card, config) ado_pcm_buf_free(card, config)
#else
#define ADO_DEVICE_MMAP(card, phys_addr, size) ado_device_mmap(phys_addr, size)
#define ADO_DEVICE_MUNMAP(card, phys_addr, size) ado_device_munmap(phys_addr, size)
#define ADO_PCM_BUF_MAP(card, config, phys_addr, size, flags) ado_pcm_buf_map(config, phys_addr, size, flags)
#define ADO_PCM_BUF_ALLOC(card, config, size, flags) ado_pcm_buf_alloc(config, size, flags)
#define ADO_PCM_BUF_FREE(card, config) ado_pcm_buf_free(config)
#endif

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/hardware/deva/ctrl/rcar/mem_map.h $ $Rev: 911782 $")
#endif
