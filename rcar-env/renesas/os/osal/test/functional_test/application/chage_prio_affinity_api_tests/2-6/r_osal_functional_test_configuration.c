/***********************************************************************************************************************
* Copyright [2023] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
*
* The contents of this file (the "contents") are proprietary and confidential to Renesas Electronics Corporation
* and/or its licensors ("Renesas") and subject to statutory and contractual protections.
*
* Unless otherwise expressly agreed in writing between Renesas and you: 1) you may not use, copy, modify, distribute,
* display, or perform the contents; 2) you may not use any name or mark of Renesas for advertising or publicity
* purposes or in connection with your use of the contents; 3) RENESAS MAKES NO WARRANTY OR REPRESENTATIONS ABOUT THE
* SUITABILITY OF THE CONTENTS FOR ANY PURPOSE; THE CONTENTS ARE PROVIDED "AS IS" WITHOUT ANY EXPRESS OR IMPLIED
* WARRANTY, INCLUDING THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, AND
* NON-INFRINGEMENT; AND 4) RENESAS SHALL NOT BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, OR CONSEQUENTIAL DAMAGES,
* INCLUDING DAMAGES RESULTING FROM LOSS OF USE, DATA, OR PROJECTS, WHETHER IN AN ACTION OF CONTRACT OR TORT, ARISING
* OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THE CONTENTS. Third-party contents included in this file may
* be subject to different terms.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name :    r_osal_functional_test_resource_info.c
* Version :      1.1.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  OSAL resource configuration file
***********************************************************************************************************************/
/***********************************************************************************************************************
 * History   Version   DD.MM.YYYY    Description
 *           1.0.0     10.07.2023    Create New
 *           1.1.0     09.11.2023    Add definition for V4M
***********************************************************************************************************************/

/*******************************************************************************************************************//**
 * includes
***********************************************************************************************************************/
#include "rcar-xos/osal/r_osal.h"
#ifndef TEST_LINUX
#include "rcar-xos/osal_configuration/target/common/r_osal_dev_cfg_info.h"
#include "rcar-xos/osal_configuration/target/common/r_osal_res_cfg_info.h"
#endif
#include "rcar-xos/osal_configuration/target/common/r_osal_mem_cfg_info.h"
#ifdef TEST_LINUX
#include "rcar-xos/osal_configuration/target/linux/r_osal_cpu_cfg_info.h"
#endif
#ifdef TEST_QNX
#include "rcar-xos/osal_configuration/target/qnx/r_osal_cpu_cfg_info.h"
#endif

#define RESORCE_ID_RESERVE  0x0000U
#define RESORCE_ID_IMR      0x1000U
#define RESORCE_ID_VIP      0x2000U
#define RESORCE_ID_ICCOM    0x3000U
#define RESORCE_ID_LDE      0x4000U
#define RESORCE_ID_CODEC    0x4000U
#define RESORCE_ID_IMP      0x5000U
#define RESORCE_ID_ISP      0x6000U
#define RESORCE_ID_OSAL     0xf000U

/*******************************************************************************************************************//**
 * @def OSAL_THREAD_ID
 * Thread IDs.
***********************************************************************************************************************/
#define    OSAL_THREAD_OSAL_00                       (RESORCE_ID_OSAL  +  0U)
#define    OSAL_THREAD_OSAL_01                       (RESORCE_ID_OSAL  +  1U)
#define    OSAL_THREAD_OSAL_02                       (RESORCE_ID_OSAL  +  2U)
#define    OSAL_THREAD_OSAL_03                       (RESORCE_ID_OSAL  +  3U)
#define    OSAL_THREAD_OSAL_04                       (RESORCE_ID_OSAL  +  4U)
#define    OSAL_THREAD_OSAL_05                       (RESORCE_ID_OSAL  +  5U)
#define    OSAL_THREAD_OSAL_06                       (RESORCE_ID_OSAL  +  6U)
#define    OSAL_THREAD_OSAL_07                       (RESORCE_ID_OSAL  +  7U)
#define    OSAL_THREAD_OSAL_08                       (RESORCE_ID_OSAL  +  8U)
#define    OSAL_THREAD_OSAL_09                       (RESORCE_ID_OSAL  +  9U)
#define    OSAL_THREAD_OSAL_10                       (RESORCE_ID_OSAL  +  10U)
#define    OSAL_THREAD_OSAL_11                       (RESORCE_ID_OSAL  +  11U)
#define    OSAL_THREAD_OSAL_12                       (RESORCE_ID_OSAL  +  12U)
#define    OSAL_THREAD_OSAL_13                       (RESORCE_ID_OSAL  +  13U)
#define    OSAL_THREAD_OSAL_14                       (RESORCE_ID_OSAL  +  14U)
#define    OSAL_THREAD_OSAL_15                       (RESORCE_ID_OSAL  +  15U)
#define    OSAL_THREAD_OSAL_16                       (RESORCE_ID_OSAL  +  16U)
#define    OSAL_THREAD_OSAL_17                       (RESORCE_ID_OSAL  +  17U)
#define    OSAL_THREAD_OSAL_18                       (RESORCE_ID_OSAL  +  18U)
#define    OSAL_THREAD_OSAL_19                       (RESORCE_ID_OSAL  +  19U)
#define    OSAL_THREAD_OSAL_20                       (RESORCE_ID_OSAL  +  20U)
#define    OSAL_THREAD_OSAL_21                       (RESORCE_ID_OSAL  +  21U)
#define    OSAL_THREAD_OSAL_22                       (RESORCE_ID_OSAL  +  22U)
#define    OSAL_THREAD_OSAL_23                       (RESORCE_ID_OSAL  +  23U)
#define    OSAL_THREAD_OSAL_24                       (RESORCE_ID_OSAL  +  24U)
#define    OSAL_THREAD_OSAL_25                       (RESORCE_ID_OSAL  +  25U)
#define    OSAL_THREAD_OSAL_26                       (RESORCE_ID_OSAL  +  26U)
#define    OSAL_THREAD_OSAL_27                       (RESORCE_ID_OSAL  +  27U)
#define    OSAL_THREAD_OSAL_28                       (RESORCE_ID_OSAL  +  28U)
#define    OSAL_THREAD_OSAL_29                       (RESORCE_ID_OSAL  +  29U)
#define    OSAL_THREAD_OSAL_30                       (RESORCE_ID_OSAL  +  30U)
#define    OSAL_THREAD_OSAL_31                       (RESORCE_ID_OSAL  +  31U)
#define    OSAL_THREAD_OSAL_32                       (RESORCE_ID_OSAL  +  32U)
#define    OSAL_THREAD_OSAL_33                       (RESORCE_ID_OSAL  +  33U)
#define    OSAL_THREAD_OSAL_34                       (RESORCE_ID_OSAL  +  34U)
#define    OSAL_THREAD_OSAL_35                       (RESORCE_ID_OSAL  +  35U)
#define    OSAL_THREAD_OSAL_36                       (RESORCE_ID_OSAL  +  36U)
#define    OSAL_THREAD_OSAL_37                       (RESORCE_ID_OSAL  +  37U)
#define    OSAL_THREAD_OSAL_38                       (RESORCE_ID_OSAL  +  38U)
#define    OSAL_THREAD_OSAL_39                       (RESORCE_ID_OSAL  +  39U)
#define    OSAL_THREAD_OSAL_40                       (RESORCE_ID_OSAL  +  40U)
#define    OSAL_THREAD_OSAL_41                       (RESORCE_ID_OSAL  +  41U)
#define    OSAL_THREAD_OSAL_42                       (RESORCE_ID_OSAL  +  42U)
#define    OSAL_THREAD_OSAL_43                       (RESORCE_ID_OSAL  +  43U)
#define    OSAL_THREAD_OSAL_44                       (RESORCE_ID_OSAL  +  44U)
#define    OSAL_THREAD_OSAL_45                       (RESORCE_ID_OSAL  +  45U)
#define    OSAL_THREAD_OSAL_46                       (RESORCE_ID_OSAL  +  46U)
#define    OSAL_THREAD_OSAL_47                       (RESORCE_ID_OSAL  +  47U)
#define    OSAL_THREAD_OSAL_48                       (RESORCE_ID_OSAL  +  48U)
#define    OSAL_THREAD_OSAL_49                       (RESORCE_ID_OSAL  +  49U)
#define    OSAL_THREAD_OSAL_50                       (RESORCE_ID_OSAL  +  50U)
#define    OSAL_THREAD_OSAL_51                       (RESORCE_ID_OSAL  +  51U)
#define    OSAL_THREAD_OSAL_52                       (RESORCE_ID_OSAL  +  52U)
#define    OSAL_THREAD_OSAL_53                       (RESORCE_ID_OSAL  +  53U)
#define    OSAL_THREAD_OSAL_54                       (RESORCE_ID_OSAL  +  54U)
#define    OSAL_THREAD_OSAL_55                       (RESORCE_ID_OSAL  +  55U)
#define    OSAL_THREAD_OSAL_56                       (RESORCE_ID_OSAL  +  56U)
#define    OSAL_THREAD_OSAL_57                       (RESORCE_ID_OSAL  +  57U)
#define    OSAL_THREAD_OSAL_58                       (RESORCE_ID_OSAL  +  58U)
#define    OSAL_THREAD_OSAL_59                       (RESORCE_ID_OSAL  +  59U)
#define    OSAL_THREAD_OSAL_60                       (RESORCE_ID_OSAL  +  60U)
#define    OSAL_THREAD_OSAL_61                       (RESORCE_ID_OSAL  +  61U)
#define    OSAL_THREAD_OSAL_62                       (RESORCE_ID_OSAL  +  62U)
#define    OSAL_THREAD_OSAL_63                       (RESORCE_ID_OSAL  +  63U)
#define    OSAL_THREAD_OSAL_64                       (RESORCE_ID_OSAL  +  64U)
#define    OSAL_THREAD_OSAL_65                       (RESORCE_ID_OSAL  +  65U)
#define    OSAL_THREAD_OSAL_66                       (RESORCE_ID_OSAL  +  66U)
#define    OSAL_THREAD_OSAL_67                       (RESORCE_ID_OSAL  +  67U)
#define    OSAL_THREAD_OSAL_68                       (RESORCE_ID_OSAL  +  68U)
#define    OSAL_THREAD_OSAL_69                       (RESORCE_ID_OSAL  +  69U)
#define    OSAL_THREAD_OSAL_70                       (RESORCE_ID_OSAL  +  70U)
#define    OSAL_THREAD_OSAL_71                       (RESORCE_ID_OSAL  +  71U)
#define    OSAL_THREAD_OSAL_72                       (RESORCE_ID_OSAL  +  72U)
#define    OSAL_THREAD_OSAL_73                       (RESORCE_ID_OSAL  +  73U)
#define    OSAL_THREAD_OSAL_74                       (RESORCE_ID_OSAL  +  74U)
#define    OSAL_THREAD_OSAL_75                       (RESORCE_ID_OSAL  +  75U)
#define    OSAL_THREAD_OSAL_76                       (RESORCE_ID_OSAL  +  76U)
#define    OSAL_THREAD_OSAL_77                       (RESORCE_ID_OSAL  +  77U)
#define    OSAL_THREAD_OSAL_78                       (RESORCE_ID_OSAL  +  78U)
#define    OSAL_THREAD_OSAL_79                       (RESORCE_ID_OSAL  +  79U)
#define    OSAL_THREAD_OSAL_80                       (RESORCE_ID_OSAL  +  80U)
#define    OSAL_THREAD_OSAL_81                       (RESORCE_ID_OSAL  +  81U)
#define    OSAL_THREAD_OSAL_82                       (RESORCE_ID_OSAL  +  82U)
#define    OSAL_THREAD_OSAL_83                       (RESORCE_ID_OSAL  +  83U)
#define    OSAL_THREAD_OSAL_84                       (RESORCE_ID_OSAL  +  84U)
#define    OSAL_THREAD_OSAL_85                       (RESORCE_ID_OSAL  +  85U)
#define    OSAL_THREAD_OSAL_86                       (RESORCE_ID_OSAL  +  86U)
#define    OSAL_THREAD_OSAL_87                       (RESORCE_ID_OSAL  +  87U)
#define    OSAL_THREAD_OSAL_88                       (RESORCE_ID_OSAL  +  88U)
#define    OSAL_THREAD_OSAL_89                       (RESORCE_ID_OSAL  +  89U)
#define    OSAL_THREAD_OSAL_90                       (RESORCE_ID_OSAL  +  90U)
#define    OSAL_THREAD_OSAL_91                       (RESORCE_ID_OSAL  +  91U)
#define    OSAL_THREAD_OSAL_92                       (RESORCE_ID_OSAL  +  92U)
#define    OSAL_THREAD_OSAL_93                       (RESORCE_ID_OSAL  +  93U)
#define    OSAL_THREAD_OSAL_94                       (RESORCE_ID_OSAL  +  94U)
#define    OSAL_THREAD_OSAL_95                       (RESORCE_ID_OSAL  +  95U)
#define    OSAL_THREAD_OSAL_96                       (RESORCE_ID_OSAL  +  96U)
#define    OSAL_THREAD_OSAL_97                       (RESORCE_ID_OSAL  +  97U)
#define    OSAL_THREAD_OSAL_98                       (RESORCE_ID_OSAL  +  98U)
#define    OSAL_THREAD_OSAL_99                       (RESORCE_ID_OSAL  +  99U)
#define    OSAL_THREAD_OSAL_100                      (RESORCE_ID_OSAL  +  100U)
#define    OSAL_THREAD_OSAL_101                      (RESORCE_ID_OSAL  +  101U)
#define    OSAL_THREAD_OSAL_102                      (RESORCE_ID_OSAL  +  102U)
#define    OSAL_THREAD_OSAL_103                      (RESORCE_ID_OSAL  +  103U)
#define    OSAL_THREAD_OSAL_104                      (RESORCE_ID_OSAL  +  104U)
#define    OSAL_THREAD_OSAL_105                      (RESORCE_ID_OSAL  +  105U)
#define    OSAL_THREAD_OSAL_106                      (RESORCE_ID_OSAL  +  106U)
#define    OSAL_THREAD_OSAL_107                      (RESORCE_ID_OSAL  +  107U)
#define    OSAL_THREAD_OSAL_108                      (RESORCE_ID_OSAL  +  108U)
#define    OSAL_THREAD_OSAL_109                      (RESORCE_ID_OSAL  +  109U)
#define    OSAL_THREAD_OSAL_110                      (RESORCE_ID_OSAL  +  110U)
#define    OSAL_THREAD_OSAL_111                      (RESORCE_ID_OSAL  +  111U)
#define    OSAL_THREAD_OSAL_112                      (RESORCE_ID_OSAL  +  112U)
#define    OSAL_THREAD_OSAL_113                      (RESORCE_ID_OSAL  +  113U)
#define    OSAL_THREAD_OSAL_114                      (RESORCE_ID_OSAL  +  114U)
#define    OSAL_THREAD_OSAL_115                      (RESORCE_ID_OSAL  +  115U)
#define    OSAL_THREAD_OSAL_116                      (RESORCE_ID_OSAL  +  116U)
#define    OSAL_THREAD_OSAL_117                      (RESORCE_ID_OSAL  +  117U)
#define    OSAL_THREAD_OSAL_118                      (RESORCE_ID_OSAL  +  118U)
#define    OSAL_THREAD_OSAL_119                      (RESORCE_ID_OSAL  +  119U)
#define    OSAL_THREAD_OSAL_120                      (RESORCE_ID_OSAL  +  120U)
#define    OSAL_THREAD_OSAL_121                      (RESORCE_ID_OSAL  +  121U)
#define    OSAL_THREAD_OSAL_122                      (RESORCE_ID_OSAL  +  122U)
#define    OSAL_THREAD_OSAL_123                      (RESORCE_ID_OSAL  +  123U)
#define    OSAL_THREAD_OSAL_124                      (RESORCE_ID_OSAL  +  124U)
#define    OSAL_THREAD_OSAL_125                      (RESORCE_ID_OSAL  +  125U)
#define    OSAL_THREAD_OSAL_126                      (RESORCE_ID_OSAL  +  126U)
#define    OSAL_THREAD_OSAL_127                      (RESORCE_ID_OSAL  +  127U)
#define    OSAL_THREAD_OSAL_128                      (RESORCE_ID_OSAL  +  128U)
#define    OSAL_THREAD_OSAL_129                      (RESORCE_ID_OSAL  +  129U)
#define    OSAL_THREAD_OSAL_130                      (RESORCE_ID_OSAL  +  130U)
#define    OSAL_THREAD_OSAL_131                      (RESORCE_ID_OSAL  +  131U)
#define    OSAL_THREAD_OSAL_132                      (RESORCE_ID_OSAL  +  132U)
#define    OSAL_THREAD_OSAL_133                      (RESORCE_ID_OSAL  +  133U)
#define    OSAL_THREAD_OSAL_134                      (RESORCE_ID_OSAL  +  134U)
#define    OSAL_THREAD_OSAL_135                      (RESORCE_ID_OSAL  +  135U)
#define    OSAL_THREAD_OSAL_136                      (RESORCE_ID_OSAL  +  136U)
#define    OSAL_THREAD_OSAL_137                      (RESORCE_ID_OSAL  +  137U)
#define    OSAL_THREAD_OSAL_138                      (RESORCE_ID_OSAL  +  138U)
#define    OSAL_THREAD_OSAL_139                      (RESORCE_ID_OSAL  +  139U)
#define    OSAL_THREAD_OSAL_140                      (RESORCE_ID_OSAL  +  140U)
#define    OSAL_THREAD_OSAL_141                      (RESORCE_ID_OSAL  +  141U)
#define    OSAL_THREAD_OSAL_142                      (RESORCE_ID_OSAL  +  142U)
#define    OSAL_THREAD_OSAL_143                      (RESORCE_ID_OSAL  +  143U)
#define    OSAL_THREAD_OSAL_144                      (RESORCE_ID_OSAL  +  144U)
#define    OSAL_THREAD_OSAL_145                      (RESORCE_ID_OSAL  +  145U)
#define    OSAL_THREAD_OSAL_146                      (RESORCE_ID_OSAL  +  146U)
#define    OSAL_THREAD_OSAL_147                      (RESORCE_ID_OSAL  +  147U)
#define    OSAL_THREAD_OSAL_148                      (RESORCE_ID_OSAL  +  148U)
#define    OSAL_THREAD_OSAL_149                      (RESORCE_ID_OSAL  +  149U)
#define    OSAL_THREAD_OSAL_150                      (RESORCE_ID_OSAL  +  150U)
#define    OSAL_THREAD_OSAL_151                      (RESORCE_ID_OSAL  +  151U)
#define    OSAL_THREAD_OSAL_152                      (RESORCE_ID_OSAL  +  152U)
#define    OSAL_THREAD_OSAL_153                      (RESORCE_ID_OSAL  +  153U)
#define    OSAL_THREAD_OSAL_154                      (RESORCE_ID_OSAL  +  154U)
#define    OSAL_THREAD_OSAL_155                      (RESORCE_ID_OSAL  +  155U)
#define    OSAL_THREAD_OSAL_156                      (RESORCE_ID_OSAL  +  156U)
#define    OSAL_THREAD_OSAL_157                      (RESORCE_ID_OSAL  +  157U)
#define    OSAL_THREAD_OSAL_158                      (RESORCE_ID_OSAL  +  158U)
#define    OSAL_THREAD_OSAL_159                      (RESORCE_ID_OSAL  +  159U)
#define    OSAL_THREAD_OSAL_160                      (RESORCE_ID_OSAL  +  160U)
#define    OSAL_THREAD_OSAL_161                      (RESORCE_ID_OSAL  +  161U)
#define    OSAL_THREAD_OSAL_162                      (RESORCE_ID_OSAL  +  162U)
#define    OSAL_THREAD_OSAL_163                      (RESORCE_ID_OSAL  +  163U)
#define    OSAL_THREAD_OSAL_164                      (RESORCE_ID_OSAL  +  164U)
#define    OSAL_THREAD_OSAL_165                      (RESORCE_ID_OSAL  +  165U)
#define    OSAL_THREAD_OSAL_166                      (RESORCE_ID_OSAL  +  166U)
#define    OSAL_THREAD_OSAL_167                      (RESORCE_ID_OSAL  +  167U)
#define    OSAL_THREAD_OSAL_168                      (RESORCE_ID_OSAL  +  168U)
#define    OSAL_THREAD_OSAL_169                      (RESORCE_ID_OSAL  +  169U)
#define    OSAL_THREAD_OSAL_170                      (RESORCE_ID_OSAL  +  170U)
#define    OSAL_THREAD_OSAL_171                      (RESORCE_ID_OSAL  +  171U)
#define    OSAL_THREAD_OSAL_172                      (RESORCE_ID_OSAL  +  172U)
#define    OSAL_THREAD_OSAL_173                      (RESORCE_ID_OSAL  +  173U)
#define    OSAL_THREAD_OSAL_174                      (RESORCE_ID_OSAL  +  174U)
#define    OSAL_THREAD_OSAL_175                      (RESORCE_ID_OSAL  +  175U)
#define    OSAL_THREAD_OSAL_176                      (RESORCE_ID_OSAL  +  176U)
#define    OSAL_THREAD_OSAL_177                      (RESORCE_ID_OSAL  +  177U)
#define    OSAL_THREAD_OSAL_178                      (RESORCE_ID_OSAL  +  178U)
#define    OSAL_THREAD_OSAL_179                      (RESORCE_ID_OSAL  +  179U)
#define    OSAL_THREAD_OSAL_180                      (RESORCE_ID_OSAL  +  180U)
#define    OSAL_THREAD_OSAL_181                      (RESORCE_ID_OSAL  +  181U)
#define    OSAL_THREAD_OSAL_182                      (RESORCE_ID_OSAL  +  182U)
#define    OSAL_THREAD_OSAL_183                      (RESORCE_ID_OSAL  +  183U)
#define    OSAL_THREAD_OSAL_184                      (RESORCE_ID_OSAL  +  184U)
#define    OSAL_THREAD_OSAL_185                      (RESORCE_ID_OSAL  +  185U)
#define    OSAL_THREAD_OSAL_186                      (RESORCE_ID_OSAL  +  186U)
#define    OSAL_THREAD_OSAL_187                      (RESORCE_ID_OSAL  +  187U)
#define    OSAL_THREAD_OSAL_188                      (RESORCE_ID_OSAL  +  188U)
#define    OSAL_THREAD_OSAL_189                      (RESORCE_ID_OSAL  +  189U)
#define    OSAL_THREAD_OSAL_190                      (RESORCE_ID_OSAL  +  190U)
#define    OSAL_THREAD_OSAL_191                      (RESORCE_ID_OSAL  +  191U)
#define    OSAL_THREAD_OSAL_192                      (RESORCE_ID_OSAL  +  192U)
#define    OSAL_THREAD_OSAL_193                      (RESORCE_ID_OSAL  +  193U)
#define    OSAL_THREAD_OSAL_194                      (RESORCE_ID_OSAL  +  194U)
#define    OSAL_THREAD_OSAL_195                      (RESORCE_ID_OSAL  +  195U)
#define    OSAL_THREAD_OSAL_196                      (RESORCE_ID_OSAL  +  196U)
#define    OSAL_THREAD_OSAL_197                      (RESORCE_ID_OSAL  +  197U)
#define    OSAL_THREAD_OSAL_198                      (RESORCE_ID_OSAL  +  198U)
#define    OSAL_THREAD_OSAL_199                      (RESORCE_ID_OSAL  +  199U)
#define    OSAL_THREAD_OSAL_200                      (RESORCE_ID_OSAL  +  200U)
#define    OSAL_THREAD_OSAL_201                      (RESORCE_ID_OSAL  +  201U)
#define    OSAL_THREAD_OSAL_202                      (RESORCE_ID_OSAL  +  202U)
#define    OSAL_THREAD_OSAL_203                      (RESORCE_ID_OSAL  +  203U)
#define    OSAL_THREAD_OSAL_204                      (RESORCE_ID_OSAL  +  204U)
#define    OSAL_THREAD_OSAL_205                      (RESORCE_ID_OSAL  +  205U)
#define    OSAL_THREAD_OSAL_206                      (RESORCE_ID_OSAL  +  206U)
#define    OSAL_THREAD_OSAL_207                      (RESORCE_ID_OSAL  +  207U)
#define    OSAL_THREAD_OSAL_208                      (RESORCE_ID_OSAL  +  208U)
#define    OSAL_THREAD_OSAL_209                      (RESORCE_ID_OSAL  +  209U)
#define    OSAL_THREAD_OSAL_210                      (RESORCE_ID_OSAL  +  210U)
#define    OSAL_THREAD_OSAL_211                      (RESORCE_ID_OSAL  +  211U)
#define    OSAL_THREAD_OSAL_212                      (RESORCE_ID_OSAL  +  212U)
#define    OSAL_THREAD_OSAL_213                      (RESORCE_ID_OSAL  +  213U)
#define    OSAL_THREAD_OSAL_214                      (RESORCE_ID_OSAL  +  214U)
#define    OSAL_THREAD_OSAL_215                      (RESORCE_ID_OSAL  +  215U)
#define    OSAL_THREAD_OSAL_216                      (RESORCE_ID_OSAL  +  216U)
#define    OSAL_THREAD_OSAL_217                      (RESORCE_ID_OSAL  +  217U)
#define    OSAL_THREAD_OSAL_218                      (RESORCE_ID_OSAL  +  218U)
#define    OSAL_THREAD_OSAL_219                      (RESORCE_ID_OSAL  +  219U)
#define    OSAL_THREAD_OSAL_220                      (RESORCE_ID_OSAL  +  220U)
#define    OSAL_THREAD_OSAL_221                      (RESORCE_ID_OSAL  +  221U)
#define    OSAL_THREAD_OSAL_222                      (RESORCE_ID_OSAL  +  222U)
#define    OSAL_THREAD_OSAL_223                      (RESORCE_ID_OSAL  +  223U)
#define    OSAL_THREAD_OSAL_224                      (RESORCE_ID_OSAL  +  224U)
#define    OSAL_THREAD_OSAL_225                      (RESORCE_ID_OSAL  +  225U)
#define    OSAL_THREAD_OSAL_226                      (RESORCE_ID_OSAL  +  226U)
#define    OSAL_THREAD_OSAL_227                      (RESORCE_ID_OSAL  +  227U)
#define    OSAL_THREAD_OSAL_228                      (RESORCE_ID_OSAL  +  228U)
#define    OSAL_THREAD_OSAL_229                      (RESORCE_ID_OSAL  +  229U)
#define    OSAL_THREAD_OSAL_230                      (RESORCE_ID_OSAL  +  230U)
#define    OSAL_THREAD_OSAL_231                      (RESORCE_ID_OSAL  +  231U)
#define    OSAL_THREAD_OSAL_232                      (RESORCE_ID_OSAL  +  232U)
#define    OSAL_THREAD_OSAL_233                      (RESORCE_ID_OSAL  +  233U)
#define    OSAL_THREAD_OSAL_234                      (RESORCE_ID_OSAL  +  234U)
#define    OSAL_THREAD_OSAL_235                      (RESORCE_ID_OSAL  +  235U)
#define    OSAL_THREAD_OSAL_236                      (RESORCE_ID_OSAL  +  236U)
#define    OSAL_THREAD_OSAL_237                      (RESORCE_ID_OSAL  +  237U)
#define    OSAL_THREAD_OSAL_238                      (RESORCE_ID_OSAL  +  238U)
#define    OSAL_THREAD_OSAL_239                      (RESORCE_ID_OSAL  +  239U)
#define    OSAL_THREAD_OSAL_240                      (RESORCE_ID_OSAL  +  240U)
#define    OSAL_THREAD_OSAL_241                      (RESORCE_ID_OSAL  +  241U)
#define    OSAL_THREAD_OSAL_242                      (RESORCE_ID_OSAL  +  242U)
#define    OSAL_THREAD_OSAL_243                      (RESORCE_ID_OSAL  +  243U)
#define    OSAL_THREAD_OSAL_244                      (RESORCE_ID_OSAL  +  244U)
#define    OSAL_THREAD_OSAL_245                      (RESORCE_ID_OSAL  +  245U)
#define    OSAL_THREAD_OSAL_246                      (RESORCE_ID_OSAL  +  246U)
#define    OSAL_THREAD_OSAL_247                      (RESORCE_ID_OSAL  +  247U)
#define    OSAL_THREAD_OSAL_248                      (RESORCE_ID_OSAL  +  248U)
#define    OSAL_THREAD_OSAL_249                      (RESORCE_ID_OSAL  +  249U)
#define    OSAL_THREAD_OSAL_250                      (RESORCE_ID_OSAL  +  250U)
#define    OSAL_THREAD_OSAL_251                      (RESORCE_ID_OSAL  +  251U)
#define    OSAL_THREAD_OSAL_252                      (RESORCE_ID_OSAL  +  252U)
#define    OSAL_THREAD_OSAL_253                      (RESORCE_ID_OSAL  +  253U)
#define    OSAL_THREAD_OSAL_254                      (RESORCE_ID_OSAL  +  254U)
#define    OSAL_THREAD_OSAL_255                      (RESORCE_ID_OSAL  +  255U)
#define    OSAL_THREAD_OSAL_256                      (RESORCE_ID_OSAL  +  256U)
#define    OSAL_THREAD_OSAL_257                      (RESORCE_ID_OSAL  +  257U)
#define    OSAL_THREAD_OSAL_258                      (RESORCE_ID_OSAL  +  258U)
#define    OSAL_THREAD_OSAL_259                      (RESORCE_ID_OSAL  +  259U)
#define    OSAL_THREAD_OSAL_260                      (RESORCE_ID_OSAL  +  260U)
#define    OSAL_THREAD_OSAL_261                      (RESORCE_ID_OSAL  +  261U)
#define    OSAL_THREAD_OSAL_262                      (RESORCE_ID_OSAL  +  262U)
#define    OSAL_THREAD_OSAL_263                      (RESORCE_ID_OSAL  +  263U)
#define    OSAL_THREAD_OSAL_264                      (RESORCE_ID_OSAL  +  264U)
#define    OSAL_THREAD_OSAL_265                      (RESORCE_ID_OSAL  +  265U)
#define    OSAL_THREAD_OSAL_266                      (RESORCE_ID_OSAL  +  266U)
#define    OSAL_THREAD_OSAL_267                      (RESORCE_ID_OSAL  +  267U)
#define    OSAL_THREAD_OSAL_268                      (RESORCE_ID_OSAL  +  268U)
#define    OSAL_THREAD_OSAL_269                      (RESORCE_ID_OSAL  +  269U)
#define    OSAL_THREAD_OSAL_270                      (RESORCE_ID_OSAL  +  270U)
#define    OSAL_THREAD_OSAL_271                      (RESORCE_ID_OSAL  +  271U)
#define    OSAL_THREAD_OSAL_272                      (RESORCE_ID_OSAL  +  272U)
#define    OSAL_THREAD_OSAL_273                      (RESORCE_ID_OSAL  +  273U)
#define    OSAL_THREAD_OSAL_274                      (RESORCE_ID_OSAL  +  274U)
#define    OSAL_THREAD_OSAL_275                      (RESORCE_ID_OSAL  +  275U)
#define    OSAL_THREAD_OSAL_276                      (RESORCE_ID_OSAL  +  276U)
#define    OSAL_THREAD_OSAL_277                      (RESORCE_ID_OSAL  +  277U)
#define    OSAL_THREAD_OSAL_278                      (RESORCE_ID_OSAL  +  278U)
#define    OSAL_THREAD_OSAL_279                      (RESORCE_ID_OSAL  +  279U)
#define    OSAL_THREAD_OSAL_280                      (RESORCE_ID_OSAL  +  280U)
#define    OSAL_THREAD_OSAL_281                      (RESORCE_ID_OSAL  +  281U)
#define    OSAL_THREAD_OSAL_282                      (RESORCE_ID_OSAL  +  282U)
#define    OSAL_THREAD_OSAL_283                      (RESORCE_ID_OSAL  +  283U)
#define    OSAL_THREAD_OSAL_284                      (RESORCE_ID_OSAL  +  284U)
#define    OSAL_THREAD_OSAL_285                      (RESORCE_ID_OSAL  +  285U)
#define    OSAL_THREAD_OSAL_286                      (RESORCE_ID_OSAL  +  286U)
#define    OSAL_THREAD_OSAL_287                      (RESORCE_ID_OSAL  +  287U)
#define    OSAL_THREAD_OSAL_288                      (RESORCE_ID_OSAL  +  288U)
#define    OSAL_THREAD_OSAL_289                      (RESORCE_ID_OSAL  +  289U)
#define    OSAL_THREAD_OSAL_290                      (RESORCE_ID_OSAL  +  290U)
#define    OSAL_THREAD_OSAL_291                      (RESORCE_ID_OSAL  +  291U)
#define    OSAL_THREAD_OSAL_292                      (RESORCE_ID_OSAL  +  292U)
#define    OSAL_THREAD_OSAL_293                      (RESORCE_ID_OSAL  +  293U)
#define    OSAL_THREAD_OSAL_294                      (RESORCE_ID_OSAL  +  294U)
#define    OSAL_THREAD_OSAL_295                      (RESORCE_ID_OSAL  +  295U)
#define    OSAL_THREAD_OSAL_296                      (RESORCE_ID_OSAL  +  296U)
#define    OSAL_THREAD_OSAL_297                      (RESORCE_ID_OSAL  +  297U)
#define    OSAL_THREAD_OSAL_298                      (RESORCE_ID_OSAL  +  298U)
#define    OSAL_THREAD_OSAL_299                      (RESORCE_ID_OSAL  +  299U)
#define    OSAL_THREAD_OSAL_300                      (RESORCE_ID_OSAL  +  300U)
#define    OSAL_THREAD_OSAL_301                      (RESORCE_ID_OSAL  +  301U)
#define    OSAL_THREAD_OSAL_302                      (RESORCE_ID_OSAL  +  302U)
#define    OSAL_THREAD_OSAL_303                      (RESORCE_ID_OSAL  +  303U)
#define    OSAL_THREAD_OSAL_304                      (RESORCE_ID_OSAL  +  304U)
#define    OSAL_THREAD_OSAL_305                      (RESORCE_ID_OSAL  +  305U)
#define    OSAL_THREAD_OSAL_306                      (RESORCE_ID_OSAL  +  306U)
#define    OSAL_THREAD_OSAL_307                      (RESORCE_ID_OSAL  +  307U)
#define    OSAL_THREAD_OSAL_308                      (RESORCE_ID_OSAL  +  308U)
#define    OSAL_THREAD_OSAL_309                      (RESORCE_ID_OSAL  +  309U)
#define    OSAL_THREAD_OSAL_310                      (RESORCE_ID_OSAL  +  310U)
#define    OSAL_THREAD_OSAL_311                      (RESORCE_ID_OSAL  +  311U)
#define    OSAL_THREAD_OSAL_312                      (RESORCE_ID_OSAL  +  312U)
#define    OSAL_THREAD_OSAL_313                      (RESORCE_ID_OSAL  +  313U)
#define    OSAL_THREAD_OSAL_314                      (RESORCE_ID_OSAL  +  314U)
#define    OSAL_THREAD_OSAL_315                      (RESORCE_ID_OSAL  +  315U)
#define    OSAL_THREAD_OSAL_316                      (RESORCE_ID_OSAL  +  316U)
#define    OSAL_THREAD_OSAL_317                      (RESORCE_ID_OSAL  +  317U)
#define    OSAL_THREAD_OSAL_318                      (RESORCE_ID_OSAL  +  318U)
#define    OSAL_THREAD_OSAL_319                      (RESORCE_ID_OSAL  +  319U)
#define    OSAL_THREAD_OSAL_320                      (RESORCE_ID_OSAL  +  320U)
#define    OSAL_THREAD_OSAL_321                      (RESORCE_ID_OSAL  +  321U)
#define    OSAL_THREAD_OSAL_322                      (RESORCE_ID_OSAL  +  322U)
#define    OSAL_THREAD_OSAL_323                      (RESORCE_ID_OSAL  +  323U)
#define    OSAL_THREAD_OSAL_324                      (RESORCE_ID_OSAL  +  324U)
#define    OSAL_THREAD_OSAL_325                      (RESORCE_ID_OSAL  +  325U)
#define    OSAL_THREAD_OSAL_326                      (RESORCE_ID_OSAL  +  326U)
#define    OSAL_THREAD_OSAL_327                      (RESORCE_ID_OSAL  +  327U)
#define    OSAL_THREAD_OSAL_328                      (RESORCE_ID_OSAL  +  328U)
#define    OSAL_THREAD_OSAL_329                      (RESORCE_ID_OSAL  +  329U)
#define    OSAL_THREAD_OSAL_330                      (RESORCE_ID_OSAL  +  330U)
#define    OSAL_THREAD_OSAL_331                      (RESORCE_ID_OSAL  +  331U)
#define    OSAL_THREAD_OSAL_332                      (RESORCE_ID_OSAL  +  332U)
#define    OSAL_THREAD_OSAL_333                      (RESORCE_ID_OSAL  +  333U)
#define    OSAL_THREAD_OSAL_334                      (RESORCE_ID_OSAL  +  334U)
#define    OSAL_THREAD_OSAL_335                      (RESORCE_ID_OSAL  +  335U)
#define    OSAL_THREAD_OSAL_336                      (RESORCE_ID_OSAL  +  336U)
#define    OSAL_THREAD_OSAL_337                      (RESORCE_ID_OSAL  +  337U)
#define    OSAL_THREAD_OSAL_338                      (RESORCE_ID_OSAL  +  338U)
#define    OSAL_THREAD_OSAL_339                      (RESORCE_ID_OSAL  +  339U)
#define    OSAL_THREAD_OSAL_340                      (RESORCE_ID_OSAL  +  340U)
#define    OSAL_THREAD_OSAL_341                      (RESORCE_ID_OSAL  +  341U)
#define    OSAL_THREAD_OSAL_342                      (RESORCE_ID_OSAL  +  342U)
#define    OSAL_THREAD_OSAL_343                      (RESORCE_ID_OSAL  +  343U)
#define    OSAL_THREAD_OSAL_344                      (RESORCE_ID_OSAL  +  344U)
#define    OSAL_THREAD_OSAL_345                      (RESORCE_ID_OSAL  +  345U)
#define    OSAL_THREAD_OSAL_346                      (RESORCE_ID_OSAL  +  346U)
#define    OSAL_THREAD_OSAL_347                      (RESORCE_ID_OSAL  +  347U)
#define    OSAL_THREAD_OSAL_348                      (RESORCE_ID_OSAL  +  348U)
#define    OSAL_THREAD_OSAL_349                      (RESORCE_ID_OSAL  +  349U)
#define    OSAL_THREAD_OSAL_350                      (RESORCE_ID_OSAL  +  350U)
#define    OSAL_THREAD_OSAL_351                      (RESORCE_ID_OSAL  +  351U)
#define    OSAL_THREAD_OSAL_352                      (RESORCE_ID_OSAL  +  352U)
#define    OSAL_THREAD_OSAL_353                      (RESORCE_ID_OSAL  +  353U)
#define    OSAL_THREAD_OSAL_354                      (RESORCE_ID_OSAL  +  354U)
#define    OSAL_THREAD_OSAL_355                      (RESORCE_ID_OSAL  +  355U)
#define    OSAL_THREAD_OSAL_356                      (RESORCE_ID_OSAL  +  356U)
#define    OSAL_THREAD_OSAL_357                      (RESORCE_ID_OSAL  +  357U)
#define    OSAL_THREAD_OSAL_358                      (RESORCE_ID_OSAL  +  358U)
#define    OSAL_THREAD_OSAL_359                      (RESORCE_ID_OSAL  +  359U)
#define    OSAL_THREAD_OSAL_360                      (RESORCE_ID_OSAL  +  360U)
#define    OSAL_THREAD_OSAL_361                      (RESORCE_ID_OSAL  +  361U)
#define    OSAL_THREAD_OSAL_362                      (RESORCE_ID_OSAL  +  362U)
#define    OSAL_THREAD_OSAL_363                      (RESORCE_ID_OSAL  +  363U)
#define    OSAL_THREAD_OSAL_364                      (RESORCE_ID_OSAL  +  364U)
#define    OSAL_THREAD_OSAL_365                      (RESORCE_ID_OSAL  +  365U)
#define    OSAL_THREAD_OSAL_366                      (RESORCE_ID_OSAL  +  366U)
#define    OSAL_THREAD_OSAL_367                      (RESORCE_ID_OSAL  +  367U)
#define    OSAL_THREAD_OSAL_368                      (RESORCE_ID_OSAL  +  368U)
#define    OSAL_THREAD_OSAL_369                      (RESORCE_ID_OSAL  +  369U)
#define    OSAL_THREAD_OSAL_370                      (RESORCE_ID_OSAL  +  370U)
#define    OSAL_THREAD_OSAL_371                      (RESORCE_ID_OSAL  +  371U)
#define    OSAL_THREAD_OSAL_372                      (RESORCE_ID_OSAL  +  372U)
#define    OSAL_THREAD_OSAL_373                      (RESORCE_ID_OSAL  +  373U)
#define    OSAL_THREAD_OSAL_374                      (RESORCE_ID_OSAL  +  374U)
#define    OSAL_THREAD_OSAL_375                      (RESORCE_ID_OSAL  +  375U)
#define    OSAL_THREAD_OSAL_376                      (RESORCE_ID_OSAL  +  376U)
#define    OSAL_THREAD_OSAL_377                      (RESORCE_ID_OSAL  +  377U)
#define    OSAL_THREAD_OSAL_378                      (RESORCE_ID_OSAL  +  378U)
#define    OSAL_THREAD_OSAL_379                      (RESORCE_ID_OSAL  +  379U)
#define    OSAL_THREAD_OSAL_380                      (RESORCE_ID_OSAL  +  380U)
#define    OSAL_THREAD_OSAL_381                      (RESORCE_ID_OSAL  +  381U)
#define    OSAL_THREAD_OSAL_382                      (RESORCE_ID_OSAL  +  382U)
#define    OSAL_THREAD_OSAL_383                      (RESORCE_ID_OSAL  +  383U)
#define    OSAL_THREAD_OSAL_384                      (RESORCE_ID_OSAL  +  384U)
#define    OSAL_THREAD_OSAL_385                      (RESORCE_ID_OSAL  +  385U)
#define    OSAL_THREAD_OSAL_386                      (RESORCE_ID_OSAL  +  386U)
#define    OSAL_THREAD_OSAL_387                      (RESORCE_ID_OSAL  +  387U)
#define    OSAL_THREAD_OSAL_388                      (RESORCE_ID_OSAL  +  388U)
#define    OSAL_THREAD_OSAL_389                      (RESORCE_ID_OSAL  +  389U)
#define    OSAL_THREAD_OSAL_390                      (RESORCE_ID_OSAL  +  390U)
#define    OSAL_THREAD_OSAL_391                      (RESORCE_ID_OSAL  +  391U)
#define    OSAL_THREAD_OSAL_392                      (RESORCE_ID_OSAL  +  392U)
#define    OSAL_THREAD_OSAL_393                      (RESORCE_ID_OSAL  +  393U)
#define    OSAL_THREAD_OSAL_394                      (RESORCE_ID_OSAL  +  394U)
#define    OSAL_THREAD_OSAL_395                      (RESORCE_ID_OSAL  +  395U)
#define    OSAL_THREAD_OSAL_396                      (RESORCE_ID_OSAL  +  396U)
#define    OSAL_THREAD_OSAL_397                      (RESORCE_ID_OSAL  +  397U)
#define    OSAL_THREAD_OSAL_398                      (RESORCE_ID_OSAL  +  398U)
#define    OSAL_THREAD_OSAL_399                      (RESORCE_ID_OSAL  +  399U)
#define    OSAL_THREAD_OSAL_400                      (RESORCE_ID_OSAL  +  400U)
#define    OSAL_THREAD_OSAL_401                      (RESORCE_ID_OSAL  +  401U)
#define    OSAL_THREAD_OSAL_402                      (RESORCE_ID_OSAL  +  402U)
#define    OSAL_THREAD_OSAL_403                      (RESORCE_ID_OSAL  +  403U)
#define    OSAL_THREAD_OSAL_404                      (RESORCE_ID_OSAL  +  404U)
#define    OSAL_THREAD_OSAL_405                      (RESORCE_ID_OSAL  +  405U)
#define    OSAL_THREAD_OSAL_406                      (RESORCE_ID_OSAL  +  406U)
#define    OSAL_THREAD_OSAL_407                      (RESORCE_ID_OSAL  +  407U)
#define    OSAL_THREAD_OSAL_408                      (RESORCE_ID_OSAL  +  408U)
#define    OSAL_THREAD_OSAL_409                      (RESORCE_ID_OSAL  +  409U)
#define    OSAL_THREAD_OSAL_410                      (RESORCE_ID_OSAL  +  410U)
#define    OSAL_THREAD_OSAL_411                      (RESORCE_ID_OSAL  +  411U)
#define    OSAL_THREAD_OSAL_412                      (RESORCE_ID_OSAL  +  412U)
#define    OSAL_THREAD_OSAL_413                      (RESORCE_ID_OSAL  +  413U)
#define    OSAL_THREAD_OSAL_414                      (RESORCE_ID_OSAL  +  414U)
#define    OSAL_THREAD_OSAL_415                      (RESORCE_ID_OSAL  +  415U)
#define    OSAL_THREAD_OSAL_416                      (RESORCE_ID_OSAL  +  416U)
#define    OSAL_THREAD_OSAL_417                      (RESORCE_ID_OSAL  +  417U)
#define    OSAL_THREAD_OSAL_418                      (RESORCE_ID_OSAL  +  418U)
#define    OSAL_THREAD_OSAL_419                      (RESORCE_ID_OSAL  +  419U)
#define    OSAL_THREAD_OSAL_420                      (RESORCE_ID_OSAL  +  420U)
#define    OSAL_THREAD_OSAL_421                      (RESORCE_ID_OSAL  +  421U)
#define    OSAL_THREAD_OSAL_422                      (RESORCE_ID_OSAL  +  422U)
#define    OSAL_THREAD_OSAL_423                      (RESORCE_ID_OSAL  +  423U)
#define    OSAL_THREAD_OSAL_424                      (RESORCE_ID_OSAL  +  424U)
#define    OSAL_THREAD_OSAL_425                      (RESORCE_ID_OSAL  +  425U)
#define    OSAL_THREAD_OSAL_426                      (RESORCE_ID_OSAL  +  426U)
#define    OSAL_THREAD_OSAL_427                      (RESORCE_ID_OSAL  +  427U)
#define    OSAL_THREAD_OSAL_428                      (RESORCE_ID_OSAL  +  428U)
#define    OSAL_THREAD_OSAL_429                      (RESORCE_ID_OSAL  +  429U)
#define    OSAL_THREAD_OSAL_430                      (RESORCE_ID_OSAL  +  430U)
#define    OSAL_THREAD_OSAL_431                      (RESORCE_ID_OSAL  +  431U)
#define    OSAL_THREAD_OSAL_432                      (RESORCE_ID_OSAL  +  432U)
#define    OSAL_THREAD_OSAL_433                      (RESORCE_ID_OSAL  +  433U)
#define    OSAL_THREAD_OSAL_434                      (RESORCE_ID_OSAL  +  434U)
#define    OSAL_THREAD_OSAL_435                      (RESORCE_ID_OSAL  +  435U)
#define    OSAL_THREAD_OSAL_436                      (RESORCE_ID_OSAL  +  436U)
#define    OSAL_THREAD_OSAL_437                      (RESORCE_ID_OSAL  +  437U)
#define    OSAL_THREAD_OSAL_438                      (RESORCE_ID_OSAL  +  438U)
#define    OSAL_THREAD_OSAL_439                      (RESORCE_ID_OSAL  +  439U)
#define    OSAL_THREAD_OSAL_440                      (RESORCE_ID_OSAL  +  440U)
#define    OSAL_THREAD_OSAL_441                      (RESORCE_ID_OSAL  +  441U)
#define    OSAL_THREAD_OSAL_442                      (RESORCE_ID_OSAL  +  442U)
#define    OSAL_THREAD_OSAL_443                      (RESORCE_ID_OSAL  +  443U)
#define    OSAL_THREAD_OSAL_444                      (RESORCE_ID_OSAL  +  444U)
#define    OSAL_THREAD_OSAL_445                      (RESORCE_ID_OSAL  +  445U)
#define    OSAL_THREAD_OSAL_446                      (RESORCE_ID_OSAL  +  446U)
#define    OSAL_THREAD_OSAL_447                      (RESORCE_ID_OSAL  +  447U)
#define    OSAL_THREAD_OSAL_448                      (RESORCE_ID_OSAL  +  448U)
#define    OSAL_THREAD_OSAL_449                      (RESORCE_ID_OSAL  +  449U)
#define    OSAL_THREAD_OSAL_450                      (RESORCE_ID_OSAL  +  450U)
#define    OSAL_THREAD_OSAL_451                      (RESORCE_ID_OSAL  +  451U)
#define    OSAL_THREAD_OSAL_452                      (RESORCE_ID_OSAL  +  452U)
#define    OSAL_THREAD_OSAL_453                      (RESORCE_ID_OSAL  +  453U)
#define    OSAL_THREAD_OSAL_454                      (RESORCE_ID_OSAL  +  454U)
#define    OSAL_THREAD_OSAL_455                      (RESORCE_ID_OSAL  +  455U)
#define    OSAL_THREAD_OSAL_456                      (RESORCE_ID_OSAL  +  456U)
#define    OSAL_THREAD_OSAL_457                      (RESORCE_ID_OSAL  +  457U)
#define    OSAL_THREAD_OSAL_458                      (RESORCE_ID_OSAL  +  458U)
#define    OSAL_THREAD_OSAL_459                      (RESORCE_ID_OSAL  +  459U)
#define    OSAL_THREAD_OSAL_460                      (RESORCE_ID_OSAL  +  460U)
#define    OSAL_THREAD_OSAL_461                      (RESORCE_ID_OSAL  +  461U)
#define    OSAL_THREAD_OSAL_462                      (RESORCE_ID_OSAL  +  462U)
#define    OSAL_THREAD_OSAL_463                      (RESORCE_ID_OSAL  +  463U)
#define    OSAL_THREAD_OSAL_464                      (RESORCE_ID_OSAL  +  464U)
#define    OSAL_THREAD_OSAL_465                      (RESORCE_ID_OSAL  +  465U)
#define    OSAL_THREAD_OSAL_466                      (RESORCE_ID_OSAL  +  466U)
#define    OSAL_THREAD_OSAL_467                      (RESORCE_ID_OSAL  +  467U)
#define    OSAL_THREAD_OSAL_468                      (RESORCE_ID_OSAL  +  468U)
#define    OSAL_THREAD_OSAL_469                      (RESORCE_ID_OSAL  +  469U)
#define    OSAL_THREAD_OSAL_470                      (RESORCE_ID_OSAL  +  470U)
#define    OSAL_THREAD_OSAL_471                      (RESORCE_ID_OSAL  +  471U)
#define    OSAL_THREAD_OSAL_472                      (RESORCE_ID_OSAL  +  472U)
#define    OSAL_THREAD_OSAL_473                      (RESORCE_ID_OSAL  +  473U)
#define    OSAL_THREAD_OSAL_474                      (RESORCE_ID_OSAL  +  474U)
#define    OSAL_THREAD_OSAL_475                      (RESORCE_ID_OSAL  +  475U)
#define    OSAL_THREAD_OSAL_476                      (RESORCE_ID_OSAL  +  476U)
#define    OSAL_THREAD_OSAL_477                      (RESORCE_ID_OSAL  +  477U)
#define    OSAL_THREAD_OSAL_478                      (RESORCE_ID_OSAL  +  478U)
#define    OSAL_THREAD_OSAL_479                      (RESORCE_ID_OSAL  +  479U)
#define    OSAL_THREAD_OSAL_480                      (RESORCE_ID_OSAL  +  480U)
#define    OSAL_THREAD_OSAL_481                      (RESORCE_ID_OSAL  +  481U)
#define    OSAL_THREAD_OSAL_482                      (RESORCE_ID_OSAL  +  482U)
#define    OSAL_THREAD_OSAL_483                      (RESORCE_ID_OSAL  +  483U)
#define    OSAL_THREAD_OSAL_484                      (RESORCE_ID_OSAL  +  484U)
#define    OSAL_THREAD_OSAL_485                      (RESORCE_ID_OSAL  +  485U)
#define    OSAL_THREAD_OSAL_486                      (RESORCE_ID_OSAL  +  486U)
#define    OSAL_THREAD_OSAL_487                      (RESORCE_ID_OSAL  +  487U)
#define    OSAL_THREAD_OSAL_488                      (RESORCE_ID_OSAL  +  488U)
#define    OSAL_THREAD_OSAL_489                      (RESORCE_ID_OSAL  +  489U)
#define    OSAL_THREAD_OSAL_490                      (RESORCE_ID_OSAL  +  490U)
#define    OSAL_THREAD_OSAL_491                      (RESORCE_ID_OSAL  +  491U)
#define    OSAL_THREAD_OSAL_492                      (RESORCE_ID_OSAL  +  492U)
#define    OSAL_THREAD_OSAL_493                      (RESORCE_ID_OSAL  +  493U)
#define    OSAL_THREAD_OSAL_494                      (RESORCE_ID_OSAL  +  494U)
#define    OSAL_THREAD_OSAL_495                      (RESORCE_ID_OSAL  +  495U)
#define    OSAL_THREAD_OSAL_496                      (RESORCE_ID_OSAL  +  496U)
#define    OSAL_THREAD_OSAL_497                      (RESORCE_ID_OSAL  +  497U)
#define    OSAL_THREAD_OSAL_498                      (RESORCE_ID_OSAL  +  498U)
#define    OSAL_THREAD_OSAL_499                      (RESORCE_ID_OSAL  +  499U)
#define    OSAL_THREAD_OSAL_500                      (RESORCE_ID_OSAL  +  500U)

#define    OSAL_THREAD_OSAL_PMA                      (OSAL_THREAD_INVALID_ID - 1U)

/*******************************************************************************************************************//**
 * @def OSAL_MUTEX_ID
 * Mutex IDs.
***********************************************************************************************************************/
/* xOS2 resource information */
#define    OSAL_MUTEX_OSAL_00                        (RESORCE_ID_OSAL  +  0U)
#define    OSAL_MUTEX_OSAL_01                        (RESORCE_ID_OSAL  +  1U)
#define    OSAL_MUTEX_OSAL_02                        (RESORCE_ID_OSAL  +  2U)
#define    OSAL_MUTEX_OSAL_03                        (RESORCE_ID_OSAL  +  3U)
#define    OSAL_MUTEX_OSAL_04                        (RESORCE_ID_OSAL  +  4U)
#define    OSAL_MUTEX_OSAL_05                        (RESORCE_ID_OSAL  +  5U)
#define    OSAL_MUTEX_OSAL_06                        (RESORCE_ID_OSAL  +  6U)
#define    OSAL_MUTEX_OSAL_07                        (RESORCE_ID_OSAL  +  7U)
#define    OSAL_MUTEX_OSAL_08                        (RESORCE_ID_OSAL  +  8U)
#define    OSAL_MUTEX_OSAL_09                        (RESORCE_ID_OSAL  +  9U)
#define    OSAL_MUTEX_OSAL_10                        (RESORCE_ID_OSAL  +  10U)
#define    OSAL_MUTEX_OSAL_11                        (RESORCE_ID_OSAL  +  11U)
#define    OSAL_MUTEX_OSAL_12                        (RESORCE_ID_OSAL  +  12U)
#define    OSAL_MUTEX_OSAL_13                        (RESORCE_ID_OSAL  +  13U)
#define    OSAL_MUTEX_OSAL_14                        (RESORCE_ID_OSAL  +  14U)
#define    OSAL_MUTEX_OSAL_15                        (RESORCE_ID_OSAL  +  15U)
#define    OSAL_MUTEX_OSAL_16                        (RESORCE_ID_OSAL  +  16U)
#define    OSAL_MUTEX_OSAL_17                        (RESORCE_ID_OSAL  +  17U)
#define    OSAL_MUTEX_OSAL_18                        (RESORCE_ID_OSAL  +  18U)
#define    OSAL_MUTEX_OSAL_19                        (RESORCE_ID_OSAL  +  19U)
#define    OSAL_MUTEX_OSAL_20                        (RESORCE_ID_OSAL  +  20U)
#define    OSAL_MUTEX_OSAL_21                        (RESORCE_ID_OSAL  +  21U)
#define    OSAL_MUTEX_OSAL_22                        (RESORCE_ID_OSAL  +  22U)
#define    OSAL_MUTEX_OSAL_23                        (RESORCE_ID_OSAL  +  23U)
#define    OSAL_MUTEX_OSAL_24                        (RESORCE_ID_OSAL  +  24U)
#define    OSAL_MUTEX_OSAL_25                        (RESORCE_ID_OSAL  +  25U)
#define    OSAL_MUTEX_OSAL_26                        (RESORCE_ID_OSAL  +  26U)
#define    OSAL_MUTEX_OSAL_27                        (RESORCE_ID_OSAL  +  27U)
#define    OSAL_MUTEX_OSAL_28                        (RESORCE_ID_OSAL  +  28U)
#define    OSAL_MUTEX_OSAL_29                        (RESORCE_ID_OSAL  +  29U)
#define    OSAL_MUTEX_OSAL_30                        (RESORCE_ID_OSAL  +  30U)
#define    OSAL_MUTEX_OSAL_31                        (RESORCE_ID_OSAL  +  31U)
#define    OSAL_MUTEX_OSAL_32                        (RESORCE_ID_OSAL  +  32U)
#define    OSAL_MUTEX_OSAL_33                        (RESORCE_ID_OSAL  +  33U)
#define    OSAL_MUTEX_OSAL_34                        (RESORCE_ID_OSAL  +  34U)
#define    OSAL_MUTEX_OSAL_35                        (RESORCE_ID_OSAL  +  35U)
#define    OSAL_MUTEX_OSAL_36                        (RESORCE_ID_OSAL  +  36U)
#define    OSAL_MUTEX_OSAL_37                        (RESORCE_ID_OSAL  +  37U)
#define    OSAL_MUTEX_OSAL_38                        (RESORCE_ID_OSAL  +  38U)
#define    OSAL_MUTEX_OSAL_39                        (RESORCE_ID_OSAL  +  39U)
#define    OSAL_MUTEX_OSAL_40                        (RESORCE_ID_OSAL  +  40U)
#define    OSAL_MUTEX_OSAL_41                        (RESORCE_ID_OSAL  +  41U)
#define    OSAL_MUTEX_OSAL_42                        (RESORCE_ID_OSAL  +  42U)
#define    OSAL_MUTEX_OSAL_43                        (RESORCE_ID_OSAL  +  43U)
#define    OSAL_MUTEX_OSAL_44                        (RESORCE_ID_OSAL  +  44U)
#define    OSAL_MUTEX_OSAL_45                        (RESORCE_ID_OSAL  +  45U)
#define    OSAL_MUTEX_OSAL_46                        (RESORCE_ID_OSAL  +  46U)
#define    OSAL_MUTEX_OSAL_47                        (RESORCE_ID_OSAL  +  47U)
#define    OSAL_MUTEX_OSAL_48                        (RESORCE_ID_OSAL  +  48U)
#define    OSAL_MUTEX_OSAL_49                        (RESORCE_ID_OSAL  +  49U)
#define    OSAL_MUTEX_OSAL_50                        (RESORCE_ID_OSAL  +  50U)
#define    OSAL_MUTEX_OSAL_51                        (RESORCE_ID_OSAL  +  51U)
#define    OSAL_MUTEX_OSAL_52                        (RESORCE_ID_OSAL  +  52U)
#define    OSAL_MUTEX_OSAL_53                        (RESORCE_ID_OSAL  +  53U)
#define    OSAL_MUTEX_OSAL_54                        (RESORCE_ID_OSAL  +  54U)
#define    OSAL_MUTEX_OSAL_55                        (RESORCE_ID_OSAL  +  55U)
#define    OSAL_MUTEX_OSAL_56                        (RESORCE_ID_OSAL  +  56U)
#define    OSAL_MUTEX_OSAL_57                        (RESORCE_ID_OSAL  +  57U)
#define    OSAL_MUTEX_OSAL_58                        (RESORCE_ID_OSAL  +  58U)
#define    OSAL_MUTEX_OSAL_59                        (RESORCE_ID_OSAL  +  59U)
#define    OSAL_MUTEX_OSAL_60                        (RESORCE_ID_OSAL  +  60U)
#define    OSAL_MUTEX_OSAL_61                        (RESORCE_ID_OSAL  +  61U)
#define    OSAL_MUTEX_OSAL_62                        (RESORCE_ID_OSAL  +  62U)
#define    OSAL_MUTEX_OSAL_63                        (RESORCE_ID_OSAL  +  63U)
#define    OSAL_MUTEX_OSAL_64                        (RESORCE_ID_OSAL  +  64U)
#define    OSAL_MUTEX_OSAL_65                        (RESORCE_ID_OSAL  +  65U)
#define    OSAL_MUTEX_OSAL_66                        (RESORCE_ID_OSAL  +  66U)
#define    OSAL_MUTEX_OSAL_67                        (RESORCE_ID_OSAL  +  67U)
#define    OSAL_MUTEX_OSAL_68                        (RESORCE_ID_OSAL  +  68U)
#define    OSAL_MUTEX_OSAL_69                        (RESORCE_ID_OSAL  +  69U)
#define    OSAL_MUTEX_OSAL_70                        (RESORCE_ID_OSAL  +  70U)
#define    OSAL_MUTEX_OSAL_71                        (RESORCE_ID_OSAL  +  71U)
#define    OSAL_MUTEX_OSAL_72                        (RESORCE_ID_OSAL  +  72U)
#define    OSAL_MUTEX_OSAL_73                        (RESORCE_ID_OSAL  +  73U)
#define    OSAL_MUTEX_OSAL_74                        (RESORCE_ID_OSAL  +  74U)
#define    OSAL_MUTEX_OSAL_75                        (RESORCE_ID_OSAL  +  75U)
#define    OSAL_MUTEX_OSAL_76                        (RESORCE_ID_OSAL  +  76U)
#define    OSAL_MUTEX_OSAL_77                        (RESORCE_ID_OSAL  +  77U)
#define    OSAL_MUTEX_OSAL_78                        (RESORCE_ID_OSAL  +  78U)
#define    OSAL_MUTEX_OSAL_79                        (RESORCE_ID_OSAL  +  79U)
#define    OSAL_MUTEX_OSAL_80                        (RESORCE_ID_OSAL  +  80U)
#define    OSAL_MUTEX_OSAL_81                        (RESORCE_ID_OSAL  +  81U)
#define    OSAL_MUTEX_OSAL_82                        (RESORCE_ID_OSAL  +  82U)
#define    OSAL_MUTEX_OSAL_83                        (RESORCE_ID_OSAL  +  83U)
#define    OSAL_MUTEX_OSAL_84                        (RESORCE_ID_OSAL  +  84U)
#define    OSAL_MUTEX_OSAL_85                        (RESORCE_ID_OSAL  +  85U)
#define    OSAL_MUTEX_OSAL_86                        (RESORCE_ID_OSAL  +  86U)
#define    OSAL_MUTEX_OSAL_87                        (RESORCE_ID_OSAL  +  87U)
#define    OSAL_MUTEX_OSAL_88                        (RESORCE_ID_OSAL  +  88U)
#define    OSAL_MUTEX_OSAL_89                        (RESORCE_ID_OSAL  +  89U)
#define    OSAL_MUTEX_OSAL_90                        (RESORCE_ID_OSAL  +  90U)
#define    OSAL_MUTEX_OSAL_91                        (RESORCE_ID_OSAL  +  91U)
#define    OSAL_MUTEX_OSAL_92                        (RESORCE_ID_OSAL  +  92U)
#define    OSAL_MUTEX_OSAL_93                        (RESORCE_ID_OSAL  +  93U)
#define    OSAL_MUTEX_OSAL_94                        (RESORCE_ID_OSAL  +  94U)
#define    OSAL_MUTEX_OSAL_95                        (RESORCE_ID_OSAL  +  95U)
#define    OSAL_MUTEX_OSAL_96                        (RESORCE_ID_OSAL  +  96U)
#define    OSAL_MUTEX_OSAL_97                        (RESORCE_ID_OSAL  +  97U)
#define    OSAL_MUTEX_OSAL_98                        (RESORCE_ID_OSAL  +  98U)
#define    OSAL_MUTEX_OSAL_99                        (RESORCE_ID_OSAL  +  99U)
#define    OSAL_MUTEX_OSAL_100                       (RESORCE_ID_OSAL  +  100U)
#define    OSAL_MUTEX_OSAL_101                       (RESORCE_ID_OSAL  +  101U)
#define    OSAL_MUTEX_OSAL_102                       (RESORCE_ID_OSAL  +  102U)
#define    OSAL_MUTEX_OSAL_103                       (RESORCE_ID_OSAL  +  103U)
#define    OSAL_MUTEX_OSAL_104                       (RESORCE_ID_OSAL  +  104U)
#define    OSAL_MUTEX_OSAL_105                       (RESORCE_ID_OSAL  +  105U)
#define    OSAL_MUTEX_OSAL_106                       (RESORCE_ID_OSAL  +  106U)
#define    OSAL_MUTEX_OSAL_107                       (RESORCE_ID_OSAL  +  107U)
#define    OSAL_MUTEX_OSAL_108                       (RESORCE_ID_OSAL  +  108U)
#define    OSAL_MUTEX_OSAL_109                       (RESORCE_ID_OSAL  +  109U)
#define    OSAL_MUTEX_OSAL_110                       (RESORCE_ID_OSAL  +  110U)
#define    OSAL_MUTEX_OSAL_111                       (RESORCE_ID_OSAL  +  111U)
#define    OSAL_MUTEX_OSAL_112                       (RESORCE_ID_OSAL  +  112U)
#define    OSAL_MUTEX_OSAL_113                       (RESORCE_ID_OSAL  +  113U)
#define    OSAL_MUTEX_OSAL_114                       (RESORCE_ID_OSAL  +  114U)
#define    OSAL_MUTEX_OSAL_115                       (RESORCE_ID_OSAL  +  115U)
#define    OSAL_MUTEX_OSAL_116                       (RESORCE_ID_OSAL  +  116U)
#define    OSAL_MUTEX_OSAL_117                       (RESORCE_ID_OSAL  +  117U)
#define    OSAL_MUTEX_OSAL_118                       (RESORCE_ID_OSAL  +  118U)
#define    OSAL_MUTEX_OSAL_119                       (RESORCE_ID_OSAL  +  119U)
#define    OSAL_MUTEX_OSAL_120                       (RESORCE_ID_OSAL  +  120U)
#define    OSAL_MUTEX_OSAL_121                       (RESORCE_ID_OSAL  +  121U)
#define    OSAL_MUTEX_OSAL_122                       (RESORCE_ID_OSAL  +  122U)
#define    OSAL_MUTEX_OSAL_123                       (RESORCE_ID_OSAL  +  123U)
#define    OSAL_MUTEX_OSAL_124                       (RESORCE_ID_OSAL  +  124U)
#define    OSAL_MUTEX_OSAL_125                       (RESORCE_ID_OSAL  +  125U)
#define    OSAL_MUTEX_OSAL_126                       (RESORCE_ID_OSAL  +  126U)
#define    OSAL_MUTEX_OSAL_127                       (RESORCE_ID_OSAL  +  127U)
#define    OSAL_MUTEX_OSAL_128                       (RESORCE_ID_OSAL  +  128U)
#define    OSAL_MUTEX_OSAL_129                       (RESORCE_ID_OSAL  +  129U)
#define    OSAL_MUTEX_OSAL_130                       (RESORCE_ID_OSAL  +  130U)
#define    OSAL_MUTEX_OSAL_131                       (RESORCE_ID_OSAL  +  131U)
#define    OSAL_MUTEX_OSAL_132                       (RESORCE_ID_OSAL  +  132U)
#define    OSAL_MUTEX_OSAL_133                       (RESORCE_ID_OSAL  +  133U)
#define    OSAL_MUTEX_OSAL_134                       (RESORCE_ID_OSAL  +  134U)
#define    OSAL_MUTEX_OSAL_135                       (RESORCE_ID_OSAL  +  135U)
#define    OSAL_MUTEX_OSAL_136                       (RESORCE_ID_OSAL  +  136U)
#define    OSAL_MUTEX_OSAL_137                       (RESORCE_ID_OSAL  +  137U)
#define    OSAL_MUTEX_OSAL_138                       (RESORCE_ID_OSAL  +  138U)
#define    OSAL_MUTEX_OSAL_139                       (RESORCE_ID_OSAL  +  139U)
#define    OSAL_MUTEX_OSAL_140                       (RESORCE_ID_OSAL  +  140U)
#define    OSAL_MUTEX_OSAL_141                       (RESORCE_ID_OSAL  +  141U)
#define    OSAL_MUTEX_OSAL_142                       (RESORCE_ID_OSAL  +  142U)
#define    OSAL_MUTEX_OSAL_143                       (RESORCE_ID_OSAL  +  143U)
#define    OSAL_MUTEX_OSAL_144                       (RESORCE_ID_OSAL  +  144U)
#define    OSAL_MUTEX_OSAL_145                       (RESORCE_ID_OSAL  +  145U)
#define    OSAL_MUTEX_OSAL_146                       (RESORCE_ID_OSAL  +  146U)
#define    OSAL_MUTEX_OSAL_147                       (RESORCE_ID_OSAL  +  147U)
#define    OSAL_MUTEX_OSAL_148                       (RESORCE_ID_OSAL  +  148U)
#define    OSAL_MUTEX_OSAL_149                       (RESORCE_ID_OSAL  +  149U)
#define    OSAL_MUTEX_OSAL_150                       (RESORCE_ID_OSAL  +  150U)
#define    OSAL_MUTEX_OSAL_151                       (RESORCE_ID_OSAL  +  151U)
#define    OSAL_MUTEX_OSAL_152                       (RESORCE_ID_OSAL  +  152U)
#define    OSAL_MUTEX_OSAL_153                       (RESORCE_ID_OSAL  +  153U)
#define    OSAL_MUTEX_OSAL_154                       (RESORCE_ID_OSAL  +  154U)
#define    OSAL_MUTEX_OSAL_155                       (RESORCE_ID_OSAL  +  155U)
#define    OSAL_MUTEX_OSAL_156                       (RESORCE_ID_OSAL  +  156U)
#define    OSAL_MUTEX_OSAL_157                       (RESORCE_ID_OSAL  +  157U)
#define    OSAL_MUTEX_OSAL_158                       (RESORCE_ID_OSAL  +  158U)
#define    OSAL_MUTEX_OSAL_159                       (RESORCE_ID_OSAL  +  159U)
#define    OSAL_MUTEX_OSAL_160                       (RESORCE_ID_OSAL  +  160U)
#define    OSAL_MUTEX_OSAL_161                       (RESORCE_ID_OSAL  +  161U)
#define    OSAL_MUTEX_OSAL_162                       (RESORCE_ID_OSAL  +  162U)
#define    OSAL_MUTEX_OSAL_163                       (RESORCE_ID_OSAL  +  163U)
#define    OSAL_MUTEX_OSAL_164                       (RESORCE_ID_OSAL  +  164U)
#define    OSAL_MUTEX_OSAL_165                       (RESORCE_ID_OSAL  +  165U)
#define    OSAL_MUTEX_OSAL_166                       (RESORCE_ID_OSAL  +  166U)
#define    OSAL_MUTEX_OSAL_167                       (RESORCE_ID_OSAL  +  167U)
#define    OSAL_MUTEX_OSAL_168                       (RESORCE_ID_OSAL  +  168U)
#define    OSAL_MUTEX_OSAL_169                       (RESORCE_ID_OSAL  +  169U)
#define    OSAL_MUTEX_OSAL_170                       (RESORCE_ID_OSAL  +  170U)
#define    OSAL_MUTEX_OSAL_171                       (RESORCE_ID_OSAL  +  171U)
#define    OSAL_MUTEX_OSAL_172                       (RESORCE_ID_OSAL  +  172U)
#define    OSAL_MUTEX_OSAL_173                       (RESORCE_ID_OSAL  +  173U)
#define    OSAL_MUTEX_OSAL_174                       (RESORCE_ID_OSAL  +  174U)
#define    OSAL_MUTEX_OSAL_175                       (RESORCE_ID_OSAL  +  175U)
#define    OSAL_MUTEX_OSAL_176                       (RESORCE_ID_OSAL  +  176U)
#define    OSAL_MUTEX_OSAL_177                       (RESORCE_ID_OSAL  +  177U)
#define    OSAL_MUTEX_OSAL_178                       (RESORCE_ID_OSAL  +  178U)
#define    OSAL_MUTEX_OSAL_179                       (RESORCE_ID_OSAL  +  179U)
#define    OSAL_MUTEX_OSAL_180                       (RESORCE_ID_OSAL  +  180U)
#define    OSAL_MUTEX_OSAL_181                       (RESORCE_ID_OSAL  +  181U)
#define    OSAL_MUTEX_OSAL_182                       (RESORCE_ID_OSAL  +  182U)
#define    OSAL_MUTEX_OSAL_183                       (RESORCE_ID_OSAL  +  183U)
#define    OSAL_MUTEX_OSAL_184                       (RESORCE_ID_OSAL  +  184U)
#define    OSAL_MUTEX_OSAL_185                       (RESORCE_ID_OSAL  +  185U)
#define    OSAL_MUTEX_OSAL_186                       (RESORCE_ID_OSAL  +  186U)
#define    OSAL_MUTEX_OSAL_187                       (RESORCE_ID_OSAL  +  187U)
#define    OSAL_MUTEX_OSAL_188                       (RESORCE_ID_OSAL  +  188U)
#define    OSAL_MUTEX_OSAL_189                       (RESORCE_ID_OSAL  +  189U)
#define    OSAL_MUTEX_OSAL_190                       (RESORCE_ID_OSAL  +  190U)
#define    OSAL_MUTEX_OSAL_191                       (RESORCE_ID_OSAL  +  191U)
#define    OSAL_MUTEX_OSAL_192                       (RESORCE_ID_OSAL  +  192U)
#define    OSAL_MUTEX_OSAL_193                       (RESORCE_ID_OSAL  +  193U)
#define    OSAL_MUTEX_OSAL_194                       (RESORCE_ID_OSAL  +  194U)
#define    OSAL_MUTEX_OSAL_195                       (RESORCE_ID_OSAL  +  195U)
#define    OSAL_MUTEX_OSAL_196                       (RESORCE_ID_OSAL  +  196U)
#define    OSAL_MUTEX_OSAL_197                       (RESORCE_ID_OSAL  +  197U)
#define    OSAL_MUTEX_OSAL_198                       (RESORCE_ID_OSAL  +  198U)
#define    OSAL_MUTEX_OSAL_199                       (RESORCE_ID_OSAL  +  199U)
#define    OSAL_MUTEX_OSAL_200                       (RESORCE_ID_OSAL  +  200U)
#define    OSAL_MUTEX_OSAL_201                       (RESORCE_ID_OSAL  +  201U)
#define    OSAL_MUTEX_OSAL_202                       (RESORCE_ID_OSAL  +  202U)
#define    OSAL_MUTEX_OSAL_203                       (RESORCE_ID_OSAL  +  203U)
#define    OSAL_MUTEX_OSAL_204                       (RESORCE_ID_OSAL  +  204U)
#define    OSAL_MUTEX_OSAL_205                       (RESORCE_ID_OSAL  +  205U)
#define    OSAL_MUTEX_OSAL_206                       (RESORCE_ID_OSAL  +  206U)
#define    OSAL_MUTEX_OSAL_207                       (RESORCE_ID_OSAL  +  207U)
#define    OSAL_MUTEX_OSAL_208                       (RESORCE_ID_OSAL  +  208U)
#define    OSAL_MUTEX_OSAL_209                       (RESORCE_ID_OSAL  +  209U)
#define    OSAL_MUTEX_OSAL_210                       (RESORCE_ID_OSAL  +  210U)
#define    OSAL_MUTEX_OSAL_211                       (RESORCE_ID_OSAL  +  211U)
#define    OSAL_MUTEX_OSAL_212                       (RESORCE_ID_OSAL  +  212U)
#define    OSAL_MUTEX_OSAL_213                       (RESORCE_ID_OSAL  +  213U)
#define    OSAL_MUTEX_OSAL_214                       (RESORCE_ID_OSAL  +  214U)
#define    OSAL_MUTEX_OSAL_215                       (RESORCE_ID_OSAL  +  215U)
#define    OSAL_MUTEX_OSAL_216                       (RESORCE_ID_OSAL  +  216U)
#define    OSAL_MUTEX_OSAL_217                       (RESORCE_ID_OSAL  +  217U)
#define    OSAL_MUTEX_OSAL_218                       (RESORCE_ID_OSAL  +  218U)
#define    OSAL_MUTEX_OSAL_219                       (RESORCE_ID_OSAL  +  219U)
#define    OSAL_MUTEX_OSAL_220                       (RESORCE_ID_OSAL  +  220U)
#define    OSAL_MUTEX_OSAL_221                       (RESORCE_ID_OSAL  +  221U)
#define    OSAL_MUTEX_OSAL_222                       (RESORCE_ID_OSAL  +  222U)
#define    OSAL_MUTEX_OSAL_223                       (RESORCE_ID_OSAL  +  223U)
#define    OSAL_MUTEX_OSAL_224                       (RESORCE_ID_OSAL  +  224U)
#define    OSAL_MUTEX_OSAL_225                       (RESORCE_ID_OSAL  +  225U)
#define    OSAL_MUTEX_OSAL_226                       (RESORCE_ID_OSAL  +  226U)
#define    OSAL_MUTEX_OSAL_227                       (RESORCE_ID_OSAL  +  227U)
#define    OSAL_MUTEX_OSAL_228                       (RESORCE_ID_OSAL  +  228U)
#define    OSAL_MUTEX_OSAL_229                       (RESORCE_ID_OSAL  +  229U)
#define    OSAL_MUTEX_OSAL_230                       (RESORCE_ID_OSAL  +  230U)
#define    OSAL_MUTEX_OSAL_231                       (RESORCE_ID_OSAL  +  231U)
#define    OSAL_MUTEX_OSAL_232                       (RESORCE_ID_OSAL  +  232U)
#define    OSAL_MUTEX_OSAL_233                       (RESORCE_ID_OSAL  +  233U)
#define    OSAL_MUTEX_OSAL_234                       (RESORCE_ID_OSAL  +  234U)
#define    OSAL_MUTEX_OSAL_235                       (RESORCE_ID_OSAL  +  235U)
#define    OSAL_MUTEX_OSAL_236                       (RESORCE_ID_OSAL  +  236U)
#define    OSAL_MUTEX_OSAL_237                       (RESORCE_ID_OSAL  +  237U)
#define    OSAL_MUTEX_OSAL_238                       (RESORCE_ID_OSAL  +  238U)
#define    OSAL_MUTEX_OSAL_239                       (RESORCE_ID_OSAL  +  239U)
#define    OSAL_MUTEX_OSAL_240                       (RESORCE_ID_OSAL  +  240U)
#define    OSAL_MUTEX_OSAL_241                       (RESORCE_ID_OSAL  +  241U)
#define    OSAL_MUTEX_OSAL_242                       (RESORCE_ID_OSAL  +  242U)
#define    OSAL_MUTEX_OSAL_243                       (RESORCE_ID_OSAL  +  243U)
#define    OSAL_MUTEX_OSAL_244                       (RESORCE_ID_OSAL  +  244U)
#define    OSAL_MUTEX_OSAL_245                       (RESORCE_ID_OSAL  +  245U)
#define    OSAL_MUTEX_OSAL_246                       (RESORCE_ID_OSAL  +  246U)
#define    OSAL_MUTEX_OSAL_247                       (RESORCE_ID_OSAL  +  247U)
#define    OSAL_MUTEX_OSAL_248                       (RESORCE_ID_OSAL  +  248U)
#define    OSAL_MUTEX_OSAL_249                       (RESORCE_ID_OSAL  +  249U)
#define    OSAL_MUTEX_OSAL_250                       (RESORCE_ID_OSAL  +  250U)
#define    OSAL_MUTEX_OSAL_251                       (RESORCE_ID_OSAL  +  251U)
#define    OSAL_MUTEX_OSAL_252                       (RESORCE_ID_OSAL  +  252U)
#define    OSAL_MUTEX_OSAL_253                       (RESORCE_ID_OSAL  +  253U)
#define    OSAL_MUTEX_OSAL_254                       (RESORCE_ID_OSAL  +  254U)
#define    OSAL_MUTEX_OSAL_255                       (RESORCE_ID_OSAL  +  255U)
#define    OSAL_MUTEX_OSAL_256                       (RESORCE_ID_OSAL  +  256U)
#define    OSAL_MUTEX_OSAL_257                       (RESORCE_ID_OSAL  +  257U)
#define    OSAL_MUTEX_OSAL_258                       (RESORCE_ID_OSAL  +  258U)
#define    OSAL_MUTEX_OSAL_259                       (RESORCE_ID_OSAL  +  259U)
#define    OSAL_MUTEX_OSAL_260                       (RESORCE_ID_OSAL  +  260U)
#define    OSAL_MUTEX_OSAL_261                       (RESORCE_ID_OSAL  +  261U)
#define    OSAL_MUTEX_OSAL_262                       (RESORCE_ID_OSAL  +  262U)
#define    OSAL_MUTEX_OSAL_263                       (RESORCE_ID_OSAL  +  263U)
#define    OSAL_MUTEX_OSAL_264                       (RESORCE_ID_OSAL  +  264U)
#define    OSAL_MUTEX_OSAL_265                       (RESORCE_ID_OSAL  +  265U)
#define    OSAL_MUTEX_OSAL_266                       (RESORCE_ID_OSAL  +  266U)
#define    OSAL_MUTEX_OSAL_267                       (RESORCE_ID_OSAL  +  267U)
#define    OSAL_MUTEX_OSAL_268                       (RESORCE_ID_OSAL  +  268U)
#define    OSAL_MUTEX_OSAL_269                       (RESORCE_ID_OSAL  +  269U)
#define    OSAL_MUTEX_OSAL_270                       (RESORCE_ID_OSAL  +  270U)
#define    OSAL_MUTEX_OSAL_271                       (RESORCE_ID_OSAL  +  271U)
#define    OSAL_MUTEX_OSAL_272                       (RESORCE_ID_OSAL  +  272U)
#define    OSAL_MUTEX_OSAL_273                       (RESORCE_ID_OSAL  +  273U)
#define    OSAL_MUTEX_OSAL_274                       (RESORCE_ID_OSAL  +  274U)
#define    OSAL_MUTEX_OSAL_275                       (RESORCE_ID_OSAL  +  275U)
#define    OSAL_MUTEX_OSAL_276                       (RESORCE_ID_OSAL  +  276U)
#define    OSAL_MUTEX_OSAL_277                       (RESORCE_ID_OSAL  +  277U)
#define    OSAL_MUTEX_OSAL_278                       (RESORCE_ID_OSAL  +  278U)
#define    OSAL_MUTEX_OSAL_279                       (RESORCE_ID_OSAL  +  279U)
#define    OSAL_MUTEX_OSAL_280                       (RESORCE_ID_OSAL  +  280U)
#define    OSAL_MUTEX_OSAL_281                       (RESORCE_ID_OSAL  +  281U)
#define    OSAL_MUTEX_OSAL_282                       (RESORCE_ID_OSAL  +  282U)
#define    OSAL_MUTEX_OSAL_283                       (RESORCE_ID_OSAL  +  283U)
#define    OSAL_MUTEX_OSAL_284                       (RESORCE_ID_OSAL  +  284U)
#define    OSAL_MUTEX_OSAL_285                       (RESORCE_ID_OSAL  +  285U)
#define    OSAL_MUTEX_OSAL_286                       (RESORCE_ID_OSAL  +  286U)
#define    OSAL_MUTEX_OSAL_287                       (RESORCE_ID_OSAL  +  287U)
#define    OSAL_MUTEX_OSAL_288                       (RESORCE_ID_OSAL  +  288U)
#define    OSAL_MUTEX_OSAL_289                       (RESORCE_ID_OSAL  +  289U)
#define    OSAL_MUTEX_OSAL_290                       (RESORCE_ID_OSAL  +  290U)
#define    OSAL_MUTEX_OSAL_291                       (RESORCE_ID_OSAL  +  291U)
#define    OSAL_MUTEX_OSAL_292                       (RESORCE_ID_OSAL  +  292U)
#define    OSAL_MUTEX_OSAL_293                       (RESORCE_ID_OSAL  +  293U)
#define    OSAL_MUTEX_OSAL_294                       (RESORCE_ID_OSAL  +  294U)
#define    OSAL_MUTEX_OSAL_295                       (RESORCE_ID_OSAL  +  295U)
#define    OSAL_MUTEX_OSAL_296                       (RESORCE_ID_OSAL  +  296U)
#define    OSAL_MUTEX_OSAL_297                       (RESORCE_ID_OSAL  +  297U)
#define    OSAL_MUTEX_OSAL_298                       (RESORCE_ID_OSAL  +  298U)
#define    OSAL_MUTEX_OSAL_299                       (RESORCE_ID_OSAL  +  299U)
#define    OSAL_MUTEX_OSAL_300                       (RESORCE_ID_OSAL  +  300U)
#define    OSAL_MUTEX_OSAL_301                       (RESORCE_ID_OSAL  +  301U)
#define    OSAL_MUTEX_OSAL_302                       (RESORCE_ID_OSAL  +  302U)
#define    OSAL_MUTEX_OSAL_303                       (RESORCE_ID_OSAL  +  303U)
#define    OSAL_MUTEX_OSAL_304                       (RESORCE_ID_OSAL  +  304U)
#define    OSAL_MUTEX_OSAL_305                       (RESORCE_ID_OSAL  +  305U)
#define    OSAL_MUTEX_OSAL_306                       (RESORCE_ID_OSAL  +  306U)
#define    OSAL_MUTEX_OSAL_307                       (RESORCE_ID_OSAL  +  307U)
#define    OSAL_MUTEX_OSAL_308                       (RESORCE_ID_OSAL  +  308U)
#define    OSAL_MUTEX_OSAL_309                       (RESORCE_ID_OSAL  +  309U)
#define    OSAL_MUTEX_OSAL_310                       (RESORCE_ID_OSAL  +  310U)
#define    OSAL_MUTEX_OSAL_311                       (RESORCE_ID_OSAL  +  311U)
#define    OSAL_MUTEX_OSAL_312                       (RESORCE_ID_OSAL  +  312U)
#define    OSAL_MUTEX_OSAL_313                       (RESORCE_ID_OSAL  +  313U)
#define    OSAL_MUTEX_OSAL_314                       (RESORCE_ID_OSAL  +  314U)
#define    OSAL_MUTEX_OSAL_315                       (RESORCE_ID_OSAL  +  315U)
#define    OSAL_MUTEX_OSAL_316                       (RESORCE_ID_OSAL  +  316U)
#define    OSAL_MUTEX_OSAL_317                       (RESORCE_ID_OSAL  +  317U)
#define    OSAL_MUTEX_OSAL_318                       (RESORCE_ID_OSAL  +  318U)
#define    OSAL_MUTEX_OSAL_319                       (RESORCE_ID_OSAL  +  319U)
#define    OSAL_MUTEX_OSAL_320                       (RESORCE_ID_OSAL  +  320U)
#define    OSAL_MUTEX_OSAL_321                       (RESORCE_ID_OSAL  +  321U)
#define    OSAL_MUTEX_OSAL_322                       (RESORCE_ID_OSAL  +  322U)
#define    OSAL_MUTEX_OSAL_323                       (RESORCE_ID_OSAL  +  323U)
#define    OSAL_MUTEX_OSAL_324                       (RESORCE_ID_OSAL  +  324U)
#define    OSAL_MUTEX_OSAL_325                       (RESORCE_ID_OSAL  +  325U)
#define    OSAL_MUTEX_OSAL_326                       (RESORCE_ID_OSAL  +  326U)
#define    OSAL_MUTEX_OSAL_327                       (RESORCE_ID_OSAL  +  327U)
#define    OSAL_MUTEX_OSAL_328                       (RESORCE_ID_OSAL  +  328U)
#define    OSAL_MUTEX_OSAL_329                       (RESORCE_ID_OSAL  +  329U)
#define    OSAL_MUTEX_OSAL_330                       (RESORCE_ID_OSAL  +  330U)
#define    OSAL_MUTEX_OSAL_331                       (RESORCE_ID_OSAL  +  331U)
#define    OSAL_MUTEX_OSAL_332                       (RESORCE_ID_OSAL  +  332U)
#define    OSAL_MUTEX_OSAL_333                       (RESORCE_ID_OSAL  +  333U)
#define    OSAL_MUTEX_OSAL_334                       (RESORCE_ID_OSAL  +  334U)
#define    OSAL_MUTEX_OSAL_335                       (RESORCE_ID_OSAL  +  335U)
#define    OSAL_MUTEX_OSAL_336                       (RESORCE_ID_OSAL  +  336U)
#define    OSAL_MUTEX_OSAL_337                       (RESORCE_ID_OSAL  +  337U)
#define    OSAL_MUTEX_OSAL_338                       (RESORCE_ID_OSAL  +  338U)
#define    OSAL_MUTEX_OSAL_339                       (RESORCE_ID_OSAL  +  339U)
#define    OSAL_MUTEX_OSAL_340                       (RESORCE_ID_OSAL  +  340U)
#define    OSAL_MUTEX_OSAL_341                       (RESORCE_ID_OSAL  +  341U)
#define    OSAL_MUTEX_OSAL_342                       (RESORCE_ID_OSAL  +  342U)
#define    OSAL_MUTEX_OSAL_343                       (RESORCE_ID_OSAL  +  343U)
#define    OSAL_MUTEX_OSAL_344                       (RESORCE_ID_OSAL  +  344U)
#define    OSAL_MUTEX_OSAL_345                       (RESORCE_ID_OSAL  +  345U)
#define    OSAL_MUTEX_OSAL_346                       (RESORCE_ID_OSAL  +  346U)
#define    OSAL_MUTEX_OSAL_347                       (RESORCE_ID_OSAL  +  347U)
#define    OSAL_MUTEX_OSAL_348                       (RESORCE_ID_OSAL  +  348U)
#define    OSAL_MUTEX_OSAL_349                       (RESORCE_ID_OSAL  +  349U)
#define    OSAL_MUTEX_OSAL_350                       (RESORCE_ID_OSAL  +  350U)
#define    OSAL_MUTEX_OSAL_351                       (RESORCE_ID_OSAL  +  351U)
#define    OSAL_MUTEX_OSAL_352                       (RESORCE_ID_OSAL  +  352U)
#define    OSAL_MUTEX_OSAL_353                       (RESORCE_ID_OSAL  +  353U)
#define    OSAL_MUTEX_OSAL_354                       (RESORCE_ID_OSAL  +  354U)
#define    OSAL_MUTEX_OSAL_355                       (RESORCE_ID_OSAL  +  355U)
#define    OSAL_MUTEX_OSAL_356                       (RESORCE_ID_OSAL  +  356U)
#define    OSAL_MUTEX_OSAL_357                       (RESORCE_ID_OSAL  +  357U)
#define    OSAL_MUTEX_OSAL_358                       (RESORCE_ID_OSAL  +  358U)
#define    OSAL_MUTEX_OSAL_359                       (RESORCE_ID_OSAL  +  359U)
#define    OSAL_MUTEX_OSAL_360                       (RESORCE_ID_OSAL  +  360U)
#define    OSAL_MUTEX_OSAL_361                       (RESORCE_ID_OSAL  +  361U)
#define    OSAL_MUTEX_OSAL_362                       (RESORCE_ID_OSAL  +  362U)
#define    OSAL_MUTEX_OSAL_363                       (RESORCE_ID_OSAL  +  363U)
#define    OSAL_MUTEX_OSAL_364                       (RESORCE_ID_OSAL  +  364U)
#define    OSAL_MUTEX_OSAL_365                       (RESORCE_ID_OSAL  +  365U)
#define    OSAL_MUTEX_OSAL_366                       (RESORCE_ID_OSAL  +  366U)
#define    OSAL_MUTEX_OSAL_367                       (RESORCE_ID_OSAL  +  367U)
#define    OSAL_MUTEX_OSAL_368                       (RESORCE_ID_OSAL  +  368U)
#define    OSAL_MUTEX_OSAL_369                       (RESORCE_ID_OSAL  +  369U)
#define    OSAL_MUTEX_OSAL_370                       (RESORCE_ID_OSAL  +  370U)
#define    OSAL_MUTEX_OSAL_371                       (RESORCE_ID_OSAL  +  371U)
#define    OSAL_MUTEX_OSAL_372                       (RESORCE_ID_OSAL  +  372U)
#define    OSAL_MUTEX_OSAL_373                       (RESORCE_ID_OSAL  +  373U)
#define    OSAL_MUTEX_OSAL_374                       (RESORCE_ID_OSAL  +  374U)
#define    OSAL_MUTEX_OSAL_375                       (RESORCE_ID_OSAL  +  375U)
#define    OSAL_MUTEX_OSAL_376                       (RESORCE_ID_OSAL  +  376U)
#define    OSAL_MUTEX_OSAL_377                       (RESORCE_ID_OSAL  +  377U)
#define    OSAL_MUTEX_OSAL_378                       (RESORCE_ID_OSAL  +  378U)
#define    OSAL_MUTEX_OSAL_379                       (RESORCE_ID_OSAL  +  379U)
#define    OSAL_MUTEX_OSAL_380                       (RESORCE_ID_OSAL  +  380U)
#define    OSAL_MUTEX_OSAL_381                       (RESORCE_ID_OSAL  +  381U)
#define    OSAL_MUTEX_OSAL_382                       (RESORCE_ID_OSAL  +  382U)
#define    OSAL_MUTEX_OSAL_383                       (RESORCE_ID_OSAL  +  383U)
#define    OSAL_MUTEX_OSAL_384                       (RESORCE_ID_OSAL  +  384U)
#define    OSAL_MUTEX_OSAL_385                       (RESORCE_ID_OSAL  +  385U)
#define    OSAL_MUTEX_OSAL_386                       (RESORCE_ID_OSAL  +  386U)
#define    OSAL_MUTEX_OSAL_387                       (RESORCE_ID_OSAL  +  387U)
#define    OSAL_MUTEX_OSAL_388                       (RESORCE_ID_OSAL  +  388U)
#define    OSAL_MUTEX_OSAL_389                       (RESORCE_ID_OSAL  +  389U)
#define    OSAL_MUTEX_OSAL_390                       (RESORCE_ID_OSAL  +  390U)
#define    OSAL_MUTEX_OSAL_391                       (RESORCE_ID_OSAL  +  391U)
#define    OSAL_MUTEX_OSAL_392                       (RESORCE_ID_OSAL  +  392U)
#define    OSAL_MUTEX_OSAL_393                       (RESORCE_ID_OSAL  +  393U)
#define    OSAL_MUTEX_OSAL_394                       (RESORCE_ID_OSAL  +  394U)
#define    OSAL_MUTEX_OSAL_395                       (RESORCE_ID_OSAL  +  395U)
#define    OSAL_MUTEX_OSAL_396                       (RESORCE_ID_OSAL  +  396U)
#define    OSAL_MUTEX_OSAL_397                       (RESORCE_ID_OSAL  +  397U)
#define    OSAL_MUTEX_OSAL_398                       (RESORCE_ID_OSAL  +  398U)
#define    OSAL_MUTEX_OSAL_399                       (RESORCE_ID_OSAL  +  399U)
#define    OSAL_MUTEX_OSAL_400                       (RESORCE_ID_OSAL  +  400U)
#define    OSAL_MUTEX_OSAL_401                       (RESORCE_ID_OSAL  +  401U)
#define    OSAL_MUTEX_OSAL_402                       (RESORCE_ID_OSAL  +  402U)
#define    OSAL_MUTEX_OSAL_403                       (RESORCE_ID_OSAL  +  403U)
#define    OSAL_MUTEX_OSAL_404                       (RESORCE_ID_OSAL  +  404U)
#define    OSAL_MUTEX_OSAL_405                       (RESORCE_ID_OSAL  +  405U)
#define    OSAL_MUTEX_OSAL_406                       (RESORCE_ID_OSAL  +  406U)
#define    OSAL_MUTEX_OSAL_407                       (RESORCE_ID_OSAL  +  407U)
#define    OSAL_MUTEX_OSAL_408                       (RESORCE_ID_OSAL  +  408U)
#define    OSAL_MUTEX_OSAL_409                       (RESORCE_ID_OSAL  +  409U)
#define    OSAL_MUTEX_OSAL_410                       (RESORCE_ID_OSAL  +  410U)
#define    OSAL_MUTEX_OSAL_411                       (RESORCE_ID_OSAL  +  411U)
#define    OSAL_MUTEX_OSAL_412                       (RESORCE_ID_OSAL  +  412U)
#define    OSAL_MUTEX_OSAL_413                       (RESORCE_ID_OSAL  +  413U)
#define    OSAL_MUTEX_OSAL_414                       (RESORCE_ID_OSAL  +  414U)
#define    OSAL_MUTEX_OSAL_415                       (RESORCE_ID_OSAL  +  415U)
#define    OSAL_MUTEX_OSAL_416                       (RESORCE_ID_OSAL  +  416U)
#define    OSAL_MUTEX_OSAL_417                       (RESORCE_ID_OSAL  +  417U)
#define    OSAL_MUTEX_OSAL_418                       (RESORCE_ID_OSAL  +  418U)
#define    OSAL_MUTEX_OSAL_419                       (RESORCE_ID_OSAL  +  419U)
#define    OSAL_MUTEX_OSAL_420                       (RESORCE_ID_OSAL  +  420U)
#define    OSAL_MUTEX_OSAL_421                       (RESORCE_ID_OSAL  +  421U)
#define    OSAL_MUTEX_OSAL_422                       (RESORCE_ID_OSAL  +  422U)
#define    OSAL_MUTEX_OSAL_423                       (RESORCE_ID_OSAL  +  423U)
#define    OSAL_MUTEX_OSAL_424                       (RESORCE_ID_OSAL  +  424U)
#define    OSAL_MUTEX_OSAL_425                       (RESORCE_ID_OSAL  +  425U)
#define    OSAL_MUTEX_OSAL_426                       (RESORCE_ID_OSAL  +  426U)
#define    OSAL_MUTEX_OSAL_427                       (RESORCE_ID_OSAL  +  427U)
#define    OSAL_MUTEX_OSAL_428                       (RESORCE_ID_OSAL  +  428U)
#define    OSAL_MUTEX_OSAL_429                       (RESORCE_ID_OSAL  +  429U)
#define    OSAL_MUTEX_OSAL_430                       (RESORCE_ID_OSAL  +  430U)
#define    OSAL_MUTEX_OSAL_431                       (RESORCE_ID_OSAL  +  431U)
#define    OSAL_MUTEX_OSAL_432                       (RESORCE_ID_OSAL  +  432U)
#define    OSAL_MUTEX_OSAL_433                       (RESORCE_ID_OSAL  +  433U)
#define    OSAL_MUTEX_OSAL_434                       (RESORCE_ID_OSAL  +  434U)
#define    OSAL_MUTEX_OSAL_435                       (RESORCE_ID_OSAL  +  435U)
#define    OSAL_MUTEX_OSAL_436                       (RESORCE_ID_OSAL  +  436U)
#define    OSAL_MUTEX_OSAL_437                       (RESORCE_ID_OSAL  +  437U)
#define    OSAL_MUTEX_OSAL_438                       (RESORCE_ID_OSAL  +  438U)
#define    OSAL_MUTEX_OSAL_439                       (RESORCE_ID_OSAL  +  439U)
#define    OSAL_MUTEX_OSAL_440                       (RESORCE_ID_OSAL  +  440U)
#define    OSAL_MUTEX_OSAL_441                       (RESORCE_ID_OSAL  +  441U)
#define    OSAL_MUTEX_OSAL_442                       (RESORCE_ID_OSAL  +  442U)
#define    OSAL_MUTEX_OSAL_443                       (RESORCE_ID_OSAL  +  443U)
#define    OSAL_MUTEX_OSAL_444                       (RESORCE_ID_OSAL  +  444U)
#define    OSAL_MUTEX_OSAL_445                       (RESORCE_ID_OSAL  +  445U)
#define    OSAL_MUTEX_OSAL_446                       (RESORCE_ID_OSAL  +  446U)
#define    OSAL_MUTEX_OSAL_447                       (RESORCE_ID_OSAL  +  447U)
#define    OSAL_MUTEX_OSAL_448                       (RESORCE_ID_OSAL  +  448U)
#define    OSAL_MUTEX_OSAL_449                       (RESORCE_ID_OSAL  +  449U)
#define    OSAL_MUTEX_OSAL_450                       (RESORCE_ID_OSAL  +  450U)
#define    OSAL_MUTEX_OSAL_451                       (RESORCE_ID_OSAL  +  451U)
#define    OSAL_MUTEX_OSAL_452                       (RESORCE_ID_OSAL  +  452U)
#define    OSAL_MUTEX_OSAL_453                       (RESORCE_ID_OSAL  +  453U)
#define    OSAL_MUTEX_OSAL_454                       (RESORCE_ID_OSAL  +  454U)
#define    OSAL_MUTEX_OSAL_455                       (RESORCE_ID_OSAL  +  455U)
#define    OSAL_MUTEX_OSAL_456                       (RESORCE_ID_OSAL  +  456U)
#define    OSAL_MUTEX_OSAL_457                       (RESORCE_ID_OSAL  +  457U)
#define    OSAL_MUTEX_OSAL_458                       (RESORCE_ID_OSAL  +  458U)
#define    OSAL_MUTEX_OSAL_459                       (RESORCE_ID_OSAL  +  459U)
#define    OSAL_MUTEX_OSAL_460                       (RESORCE_ID_OSAL  +  460U)
#define    OSAL_MUTEX_OSAL_461                       (RESORCE_ID_OSAL  +  461U)
#define    OSAL_MUTEX_OSAL_462                       (RESORCE_ID_OSAL  +  462U)
#define    OSAL_MUTEX_OSAL_463                       (RESORCE_ID_OSAL  +  463U)
#define    OSAL_MUTEX_OSAL_464                       (RESORCE_ID_OSAL  +  464U)
#define    OSAL_MUTEX_OSAL_465                       (RESORCE_ID_OSAL  +  465U)
#define    OSAL_MUTEX_OSAL_466                       (RESORCE_ID_OSAL  +  466U)
#define    OSAL_MUTEX_OSAL_467                       (RESORCE_ID_OSAL  +  467U)
#define    OSAL_MUTEX_OSAL_468                       (RESORCE_ID_OSAL  +  468U)
#define    OSAL_MUTEX_OSAL_469                       (RESORCE_ID_OSAL  +  469U)
#define    OSAL_MUTEX_OSAL_470                       (RESORCE_ID_OSAL  +  470U)
#define    OSAL_MUTEX_OSAL_471                       (RESORCE_ID_OSAL  +  471U)
#define    OSAL_MUTEX_OSAL_472                       (RESORCE_ID_OSAL  +  472U)
#define    OSAL_MUTEX_OSAL_473                       (RESORCE_ID_OSAL  +  473U)
#define    OSAL_MUTEX_OSAL_474                       (RESORCE_ID_OSAL  +  474U)
#define    OSAL_MUTEX_OSAL_475                       (RESORCE_ID_OSAL  +  475U)
#define    OSAL_MUTEX_OSAL_476                       (RESORCE_ID_OSAL  +  476U)
#define    OSAL_MUTEX_OSAL_477                       (RESORCE_ID_OSAL  +  477U)
#define    OSAL_MUTEX_OSAL_478                       (RESORCE_ID_OSAL  +  478U)
#define    OSAL_MUTEX_OSAL_479                       (RESORCE_ID_OSAL  +  479U)
#define    OSAL_MUTEX_OSAL_480                       (RESORCE_ID_OSAL  +  480U)
#define    OSAL_MUTEX_OSAL_481                       (RESORCE_ID_OSAL  +  481U)
#define    OSAL_MUTEX_OSAL_482                       (RESORCE_ID_OSAL  +  482U)
#define    OSAL_MUTEX_OSAL_483                       (RESORCE_ID_OSAL  +  483U)
#define    OSAL_MUTEX_OSAL_484                       (RESORCE_ID_OSAL  +  484U)
#define    OSAL_MUTEX_OSAL_485                       (RESORCE_ID_OSAL  +  485U)
#define    OSAL_MUTEX_OSAL_486                       (RESORCE_ID_OSAL  +  486U)
#define    OSAL_MUTEX_OSAL_487                       (RESORCE_ID_OSAL  +  487U)
#define    OSAL_MUTEX_OSAL_488                       (RESORCE_ID_OSAL  +  488U)
#define    OSAL_MUTEX_OSAL_489                       (RESORCE_ID_OSAL  +  489U)
#define    OSAL_MUTEX_OSAL_490                       (RESORCE_ID_OSAL  +  490U)
#define    OSAL_MUTEX_OSAL_491                       (RESORCE_ID_OSAL  +  491U)
#define    OSAL_MUTEX_OSAL_492                       (RESORCE_ID_OSAL  +  492U)
#define    OSAL_MUTEX_OSAL_493                       (RESORCE_ID_OSAL  +  493U)
#define    OSAL_MUTEX_OSAL_494                       (RESORCE_ID_OSAL  +  494U)
#define    OSAL_MUTEX_OSAL_495                       (RESORCE_ID_OSAL  +  495U)
#define    OSAL_MUTEX_OSAL_496                       (RESORCE_ID_OSAL  +  496U)
#define    OSAL_MUTEX_OSAL_497                       (RESORCE_ID_OSAL  +  497U)
#define    OSAL_MUTEX_OSAL_498                       (RESORCE_ID_OSAL  +  498U)
#define    OSAL_MUTEX_OSAL_499                       (RESORCE_ID_OSAL  +  499U)
#define    OSAL_MUTEX_OSAL_500                       (RESORCE_ID_OSAL  +  500U)

#define    OSAL_MUTEX_OSAL_PMA                       (OSAL_MUTEX_INVALID_ID  -  1U)

/*******************************************************************************************************************//**
 * @def OSAL_COND_ID
 * Condition variable IDs.
***********************************************************************************************************************/
/* xOS2 resource information */
#define    OSAL_COND_OSAL_00                      (RESORCE_ID_OSAL +  0U)
#define    OSAL_COND_OSAL_01                      (RESORCE_ID_OSAL +  1U)
#define    OSAL_COND_OSAL_02                      (RESORCE_ID_OSAL +  2U)
#define    OSAL_COND_OSAL_03                      (RESORCE_ID_OSAL +  3U)
#define    OSAL_COND_OSAL_04                      (RESORCE_ID_OSAL +  4U)
#define    OSAL_COND_OSAL_05                      (RESORCE_ID_OSAL +  5U)
#define    OSAL_COND_OSAL_06                      (RESORCE_ID_OSAL +  6U)
#define    OSAL_COND_OSAL_07                      (RESORCE_ID_OSAL +  7U)
#define    OSAL_COND_OSAL_08                      (RESORCE_ID_OSAL +  8U)
#define    OSAL_COND_OSAL_09                      (RESORCE_ID_OSAL +  9U)
#define    OSAL_COND_OSAL_10                      (RESORCE_ID_OSAL +  10U)
#define    OSAL_COND_OSAL_11                      (RESORCE_ID_OSAL +  11U)
#define    OSAL_COND_OSAL_12                      (RESORCE_ID_OSAL +  12U)
#define    OSAL_COND_OSAL_13                      (RESORCE_ID_OSAL +  13U)
#define    OSAL_COND_OSAL_14                      (RESORCE_ID_OSAL +  14U)
#define    OSAL_COND_OSAL_15                      (RESORCE_ID_OSAL +  15U)
#define    OSAL_COND_OSAL_16                      (RESORCE_ID_OSAL +  16U)
#define    OSAL_COND_OSAL_17                      (RESORCE_ID_OSAL +  17U)
#define    OSAL_COND_OSAL_18                      (RESORCE_ID_OSAL +  18U)
#define    OSAL_COND_OSAL_19                      (RESORCE_ID_OSAL +  19U)
#define    OSAL_COND_OSAL_20                      (RESORCE_ID_OSAL +  20U)
#define    OSAL_COND_OSAL_21                      (RESORCE_ID_OSAL +  21U)
#define    OSAL_COND_OSAL_22                      (RESORCE_ID_OSAL +  22U)
#define    OSAL_COND_OSAL_23                      (RESORCE_ID_OSAL +  23U)
#define    OSAL_COND_OSAL_24                      (RESORCE_ID_OSAL +  24U)
#define    OSAL_COND_OSAL_25                      (RESORCE_ID_OSAL +  25U)
#define    OSAL_COND_OSAL_26                      (RESORCE_ID_OSAL +  26U)
#define    OSAL_COND_OSAL_27                      (RESORCE_ID_OSAL +  27U)
#define    OSAL_COND_OSAL_28                      (RESORCE_ID_OSAL +  28U)
#define    OSAL_COND_OSAL_29                      (RESORCE_ID_OSAL +  29U)
#define    OSAL_COND_OSAL_30                      (RESORCE_ID_OSAL +  30U)
#define    OSAL_COND_OSAL_31                      (RESORCE_ID_OSAL +  31U)
#define    OSAL_COND_OSAL_32                      (RESORCE_ID_OSAL +  32U)
#define    OSAL_COND_OSAL_33                      (RESORCE_ID_OSAL +  33U)
#define    OSAL_COND_OSAL_34                      (RESORCE_ID_OSAL +  34U)
#define    OSAL_COND_OSAL_35                      (RESORCE_ID_OSAL +  35U)
#define    OSAL_COND_OSAL_36                      (RESORCE_ID_OSAL +  36U)
#define    OSAL_COND_OSAL_37                      (RESORCE_ID_OSAL +  37U)
#define    OSAL_COND_OSAL_38                      (RESORCE_ID_OSAL +  38U)
#define    OSAL_COND_OSAL_39                      (RESORCE_ID_OSAL +  39U)
#define    OSAL_COND_OSAL_40                      (RESORCE_ID_OSAL +  40U)
#define    OSAL_COND_OSAL_41                      (RESORCE_ID_OSAL +  41U)
#define    OSAL_COND_OSAL_42                      (RESORCE_ID_OSAL +  42U)
#define    OSAL_COND_OSAL_43                      (RESORCE_ID_OSAL +  43U)
#define    OSAL_COND_OSAL_44                      (RESORCE_ID_OSAL +  44U)
#define    OSAL_COND_OSAL_45                      (RESORCE_ID_OSAL +  45U)
#define    OSAL_COND_OSAL_46                      (RESORCE_ID_OSAL +  46U)
#define    OSAL_COND_OSAL_47                      (RESORCE_ID_OSAL +  47U)
#define    OSAL_COND_OSAL_48                      (RESORCE_ID_OSAL +  48U)
#define    OSAL_COND_OSAL_49                      (RESORCE_ID_OSAL +  49U)
#define    OSAL_COND_OSAL_50                      (RESORCE_ID_OSAL +  50U)
#define    OSAL_COND_OSAL_51                      (RESORCE_ID_OSAL +  51U)
#define    OSAL_COND_OSAL_52                      (RESORCE_ID_OSAL +  52U)
#define    OSAL_COND_OSAL_53                      (RESORCE_ID_OSAL +  53U)
#define    OSAL_COND_OSAL_54                      (RESORCE_ID_OSAL +  54U)
#define    OSAL_COND_OSAL_55                      (RESORCE_ID_OSAL +  55U)
#define    OSAL_COND_OSAL_56                      (RESORCE_ID_OSAL +  56U)
#define    OSAL_COND_OSAL_57                      (RESORCE_ID_OSAL +  57U)
#define    OSAL_COND_OSAL_58                      (RESORCE_ID_OSAL +  58U)
#define    OSAL_COND_OSAL_59                      (RESORCE_ID_OSAL +  59U)
#define    OSAL_COND_OSAL_60                      (RESORCE_ID_OSAL +  60U)
#define    OSAL_COND_OSAL_61                      (RESORCE_ID_OSAL +  61U)
#define    OSAL_COND_OSAL_62                      (RESORCE_ID_OSAL +  62U)
#define    OSAL_COND_OSAL_63                      (RESORCE_ID_OSAL +  63U)
#define    OSAL_COND_OSAL_64                      (RESORCE_ID_OSAL +  64U)
#define    OSAL_COND_OSAL_65                      (RESORCE_ID_OSAL +  65U)
#define    OSAL_COND_OSAL_66                      (RESORCE_ID_OSAL +  66U)
#define    OSAL_COND_OSAL_67                      (RESORCE_ID_OSAL +  67U)
#define    OSAL_COND_OSAL_68                      (RESORCE_ID_OSAL +  68U)
#define    OSAL_COND_OSAL_69                      (RESORCE_ID_OSAL +  69U)
#define    OSAL_COND_OSAL_70                      (RESORCE_ID_OSAL +  70U)
#define    OSAL_COND_OSAL_71                      (RESORCE_ID_OSAL +  71U)
#define    OSAL_COND_OSAL_72                      (RESORCE_ID_OSAL +  72U)
#define    OSAL_COND_OSAL_73                      (RESORCE_ID_OSAL +  73U)
#define    OSAL_COND_OSAL_74                      (RESORCE_ID_OSAL +  74U)
#define    OSAL_COND_OSAL_75                      (RESORCE_ID_OSAL +  75U)
#define    OSAL_COND_OSAL_76                      (RESORCE_ID_OSAL +  76U)
#define    OSAL_COND_OSAL_77                      (RESORCE_ID_OSAL +  77U)
#define    OSAL_COND_OSAL_78                      (RESORCE_ID_OSAL +  78U)
#define    OSAL_COND_OSAL_79                      (RESORCE_ID_OSAL +  79U)
#define    OSAL_COND_OSAL_80                      (RESORCE_ID_OSAL +  80U)
#define    OSAL_COND_OSAL_81                      (RESORCE_ID_OSAL +  81U)
#define    OSAL_COND_OSAL_82                      (RESORCE_ID_OSAL +  82U)
#define    OSAL_COND_OSAL_83                      (RESORCE_ID_OSAL +  83U)
#define    OSAL_COND_OSAL_84                      (RESORCE_ID_OSAL +  84U)
#define    OSAL_COND_OSAL_85                      (RESORCE_ID_OSAL +  85U)
#define    OSAL_COND_OSAL_86                      (RESORCE_ID_OSAL +  86U)
#define    OSAL_COND_OSAL_87                      (RESORCE_ID_OSAL +  87U)
#define    OSAL_COND_OSAL_88                      (RESORCE_ID_OSAL +  88U)
#define    OSAL_COND_OSAL_89                      (RESORCE_ID_OSAL +  89U)
#define    OSAL_COND_OSAL_90                      (RESORCE_ID_OSAL +  90U)
#define    OSAL_COND_OSAL_91                      (RESORCE_ID_OSAL +  91U)
#define    OSAL_COND_OSAL_92                      (RESORCE_ID_OSAL +  92U)
#define    OSAL_COND_OSAL_93                      (RESORCE_ID_OSAL +  93U)
#define    OSAL_COND_OSAL_94                      (RESORCE_ID_OSAL +  94U)
#define    OSAL_COND_OSAL_95                      (RESORCE_ID_OSAL +  95U)
#define    OSAL_COND_OSAL_96                      (RESORCE_ID_OSAL +  96U)
#define    OSAL_COND_OSAL_97                      (RESORCE_ID_OSAL +  97U)
#define    OSAL_COND_OSAL_98                      (RESORCE_ID_OSAL +  98U)
#define    OSAL_COND_OSAL_99                      (RESORCE_ID_OSAL +  99U)
#define    OSAL_COND_OSAL_100                      (RESORCE_ID_OSAL +  100U)
#define    OSAL_COND_OSAL_101                      (RESORCE_ID_OSAL +  101U)
#define    OSAL_COND_OSAL_102                      (RESORCE_ID_OSAL +  102U)
#define    OSAL_COND_OSAL_103                      (RESORCE_ID_OSAL +  103U)
#define    OSAL_COND_OSAL_104                      (RESORCE_ID_OSAL +  104U)
#define    OSAL_COND_OSAL_105                      (RESORCE_ID_OSAL +  105U)
#define    OSAL_COND_OSAL_106                      (RESORCE_ID_OSAL +  106U)
#define    OSAL_COND_OSAL_107                      (RESORCE_ID_OSAL +  107U)
#define    OSAL_COND_OSAL_108                      (RESORCE_ID_OSAL +  108U)
#define    OSAL_COND_OSAL_109                      (RESORCE_ID_OSAL +  109U)
#define    OSAL_COND_OSAL_110                      (RESORCE_ID_OSAL +  110U)
#define    OSAL_COND_OSAL_111                      (RESORCE_ID_OSAL +  111U)
#define    OSAL_COND_OSAL_112                      (RESORCE_ID_OSAL +  112U)
#define    OSAL_COND_OSAL_113                      (RESORCE_ID_OSAL +  113U)
#define    OSAL_COND_OSAL_114                      (RESORCE_ID_OSAL +  114U)
#define    OSAL_COND_OSAL_115                      (RESORCE_ID_OSAL +  115U)
#define    OSAL_COND_OSAL_116                      (RESORCE_ID_OSAL +  116U)
#define    OSAL_COND_OSAL_117                      (RESORCE_ID_OSAL +  117U)
#define    OSAL_COND_OSAL_118                      (RESORCE_ID_OSAL +  118U)
#define    OSAL_COND_OSAL_119                      (RESORCE_ID_OSAL +  119U)
#define    OSAL_COND_OSAL_120                      (RESORCE_ID_OSAL +  120U)
#define    OSAL_COND_OSAL_121                      (RESORCE_ID_OSAL +  121U)
#define    OSAL_COND_OSAL_122                      (RESORCE_ID_OSAL +  122U)
#define    OSAL_COND_OSAL_123                      (RESORCE_ID_OSAL +  123U)
#define    OSAL_COND_OSAL_124                      (RESORCE_ID_OSAL +  124U)
#define    OSAL_COND_OSAL_125                      (RESORCE_ID_OSAL +  125U)
#define    OSAL_COND_OSAL_126                      (RESORCE_ID_OSAL +  126U)
#define    OSAL_COND_OSAL_127                      (RESORCE_ID_OSAL +  127U)
#define    OSAL_COND_OSAL_128                      (RESORCE_ID_OSAL +  128U)
#define    OSAL_COND_OSAL_129                      (RESORCE_ID_OSAL +  129U)
#define    OSAL_COND_OSAL_130                      (RESORCE_ID_OSAL +  130U)
#define    OSAL_COND_OSAL_131                      (RESORCE_ID_OSAL +  131U)
#define    OSAL_COND_OSAL_132                      (RESORCE_ID_OSAL +  132U)
#define    OSAL_COND_OSAL_133                      (RESORCE_ID_OSAL +  133U)
#define    OSAL_COND_OSAL_134                      (RESORCE_ID_OSAL +  134U)
#define    OSAL_COND_OSAL_135                      (RESORCE_ID_OSAL +  135U)
#define    OSAL_COND_OSAL_136                      (RESORCE_ID_OSAL +  136U)
#define    OSAL_COND_OSAL_137                      (RESORCE_ID_OSAL +  137U)
#define    OSAL_COND_OSAL_138                      (RESORCE_ID_OSAL +  138U)
#define    OSAL_COND_OSAL_139                      (RESORCE_ID_OSAL +  139U)
#define    OSAL_COND_OSAL_140                      (RESORCE_ID_OSAL +  140U)
#define    OSAL_COND_OSAL_141                      (RESORCE_ID_OSAL +  141U)
#define    OSAL_COND_OSAL_142                      (RESORCE_ID_OSAL +  142U)
#define    OSAL_COND_OSAL_143                      (RESORCE_ID_OSAL +  143U)
#define    OSAL_COND_OSAL_144                      (RESORCE_ID_OSAL +  144U)
#define    OSAL_COND_OSAL_145                      (RESORCE_ID_OSAL +  145U)
#define    OSAL_COND_OSAL_146                      (RESORCE_ID_OSAL +  146U)
#define    OSAL_COND_OSAL_147                      (RESORCE_ID_OSAL +  147U)
#define    OSAL_COND_OSAL_148                      (RESORCE_ID_OSAL +  148U)
#define    OSAL_COND_OSAL_149                      (RESORCE_ID_OSAL +  149U)
#define    OSAL_COND_OSAL_150                      (RESORCE_ID_OSAL +  150U)
#define    OSAL_COND_OSAL_151                      (RESORCE_ID_OSAL +  151U)
#define    OSAL_COND_OSAL_152                      (RESORCE_ID_OSAL +  152U)
#define    OSAL_COND_OSAL_153                      (RESORCE_ID_OSAL +  153U)
#define    OSAL_COND_OSAL_154                      (RESORCE_ID_OSAL +  154U)
#define    OSAL_COND_OSAL_155                      (RESORCE_ID_OSAL +  155U)
#define    OSAL_COND_OSAL_156                      (RESORCE_ID_OSAL +  156U)
#define    OSAL_COND_OSAL_157                      (RESORCE_ID_OSAL +  157U)
#define    OSAL_COND_OSAL_158                      (RESORCE_ID_OSAL +  158U)
#define    OSAL_COND_OSAL_159                      (RESORCE_ID_OSAL +  159U)
#define    OSAL_COND_OSAL_160                      (RESORCE_ID_OSAL +  160U)
#define    OSAL_COND_OSAL_161                      (RESORCE_ID_OSAL +  161U)
#define    OSAL_COND_OSAL_162                      (RESORCE_ID_OSAL +  162U)
#define    OSAL_COND_OSAL_163                      (RESORCE_ID_OSAL +  163U)
#define    OSAL_COND_OSAL_164                      (RESORCE_ID_OSAL +  164U)
#define    OSAL_COND_OSAL_165                      (RESORCE_ID_OSAL +  165U)
#define    OSAL_COND_OSAL_166                      (RESORCE_ID_OSAL +  166U)
#define    OSAL_COND_OSAL_167                      (RESORCE_ID_OSAL +  167U)
#define    OSAL_COND_OSAL_168                      (RESORCE_ID_OSAL +  168U)
#define    OSAL_COND_OSAL_169                      (RESORCE_ID_OSAL +  169U)
#define    OSAL_COND_OSAL_170                      (RESORCE_ID_OSAL +  170U)
#define    OSAL_COND_OSAL_171                      (RESORCE_ID_OSAL +  171U)
#define    OSAL_COND_OSAL_172                      (RESORCE_ID_OSAL +  172U)
#define    OSAL_COND_OSAL_173                      (RESORCE_ID_OSAL +  173U)
#define    OSAL_COND_OSAL_174                      (RESORCE_ID_OSAL +  174U)
#define    OSAL_COND_OSAL_175                      (RESORCE_ID_OSAL +  175U)
#define    OSAL_COND_OSAL_176                      (RESORCE_ID_OSAL +  176U)
#define    OSAL_COND_OSAL_177                      (RESORCE_ID_OSAL +  177U)
#define    OSAL_COND_OSAL_178                      (RESORCE_ID_OSAL +  178U)
#define    OSAL_COND_OSAL_179                      (RESORCE_ID_OSAL +  179U)
#define    OSAL_COND_OSAL_180                      (RESORCE_ID_OSAL +  180U)
#define    OSAL_COND_OSAL_181                      (RESORCE_ID_OSAL +  181U)
#define    OSAL_COND_OSAL_182                      (RESORCE_ID_OSAL +  182U)
#define    OSAL_COND_OSAL_183                      (RESORCE_ID_OSAL +  183U)
#define    OSAL_COND_OSAL_184                      (RESORCE_ID_OSAL +  184U)
#define    OSAL_COND_OSAL_185                      (RESORCE_ID_OSAL +  185U)
#define    OSAL_COND_OSAL_186                      (RESORCE_ID_OSAL +  186U)
#define    OSAL_COND_OSAL_187                      (RESORCE_ID_OSAL +  187U)
#define    OSAL_COND_OSAL_188                      (RESORCE_ID_OSAL +  188U)
#define    OSAL_COND_OSAL_189                      (RESORCE_ID_OSAL +  189U)
#define    OSAL_COND_OSAL_190                      (RESORCE_ID_OSAL +  190U)
#define    OSAL_COND_OSAL_191                      (RESORCE_ID_OSAL +  191U)
#define    OSAL_COND_OSAL_192                      (RESORCE_ID_OSAL +  192U)
#define    OSAL_COND_OSAL_193                      (RESORCE_ID_OSAL +  193U)
#define    OSAL_COND_OSAL_194                      (RESORCE_ID_OSAL +  194U)
#define    OSAL_COND_OSAL_195                      (RESORCE_ID_OSAL +  195U)
#define    OSAL_COND_OSAL_196                      (RESORCE_ID_OSAL +  196U)
#define    OSAL_COND_OSAL_197                      (RESORCE_ID_OSAL +  197U)
#define    OSAL_COND_OSAL_198                      (RESORCE_ID_OSAL +  198U)
#define    OSAL_COND_OSAL_199                      (RESORCE_ID_OSAL +  199U)
#define    OSAL_COND_OSAL_200                      (RESORCE_ID_OSAL +  200U)
#define    OSAL_COND_OSAL_201                      (RESORCE_ID_OSAL +  201U)
#define    OSAL_COND_OSAL_202                      (RESORCE_ID_OSAL +  202U)
#define    OSAL_COND_OSAL_203                      (RESORCE_ID_OSAL +  203U)
#define    OSAL_COND_OSAL_204                      (RESORCE_ID_OSAL +  204U)
#define    OSAL_COND_OSAL_205                      (RESORCE_ID_OSAL +  205U)
#define    OSAL_COND_OSAL_206                      (RESORCE_ID_OSAL +  206U)
#define    OSAL_COND_OSAL_207                      (RESORCE_ID_OSAL +  207U)
#define    OSAL_COND_OSAL_208                      (RESORCE_ID_OSAL +  208U)
#define    OSAL_COND_OSAL_209                      (RESORCE_ID_OSAL +  209U)
#define    OSAL_COND_OSAL_210                      (RESORCE_ID_OSAL +  210U)
#define    OSAL_COND_OSAL_211                      (RESORCE_ID_OSAL +  211U)
#define    OSAL_COND_OSAL_212                      (RESORCE_ID_OSAL +  212U)
#define    OSAL_COND_OSAL_213                      (RESORCE_ID_OSAL +  213U)
#define    OSAL_COND_OSAL_214                      (RESORCE_ID_OSAL +  214U)
#define    OSAL_COND_OSAL_215                      (RESORCE_ID_OSAL +  215U)
#define    OSAL_COND_OSAL_216                      (RESORCE_ID_OSAL +  216U)
#define    OSAL_COND_OSAL_217                      (RESORCE_ID_OSAL +  217U)
#define    OSAL_COND_OSAL_218                      (RESORCE_ID_OSAL +  218U)
#define    OSAL_COND_OSAL_219                      (RESORCE_ID_OSAL +  219U)
#define    OSAL_COND_OSAL_220                      (RESORCE_ID_OSAL +  220U)
#define    OSAL_COND_OSAL_221                      (RESORCE_ID_OSAL +  221U)
#define    OSAL_COND_OSAL_222                      (RESORCE_ID_OSAL +  222U)
#define    OSAL_COND_OSAL_223                      (RESORCE_ID_OSAL +  223U)
#define    OSAL_COND_OSAL_224                      (RESORCE_ID_OSAL +  224U)
#define    OSAL_COND_OSAL_225                      (RESORCE_ID_OSAL +  225U)
#define    OSAL_COND_OSAL_226                      (RESORCE_ID_OSAL +  226U)
#define    OSAL_COND_OSAL_227                      (RESORCE_ID_OSAL +  227U)
#define    OSAL_COND_OSAL_228                      (RESORCE_ID_OSAL +  228U)
#define    OSAL_COND_OSAL_229                      (RESORCE_ID_OSAL +  229U)
#define    OSAL_COND_OSAL_230                      (RESORCE_ID_OSAL +  230U)
#define    OSAL_COND_OSAL_231                      (RESORCE_ID_OSAL +  231U)
#define    OSAL_COND_OSAL_232                      (RESORCE_ID_OSAL +  232U)
#define    OSAL_COND_OSAL_233                      (RESORCE_ID_OSAL +  233U)
#define    OSAL_COND_OSAL_234                      (RESORCE_ID_OSAL +  234U)
#define    OSAL_COND_OSAL_235                      (RESORCE_ID_OSAL +  235U)
#define    OSAL_COND_OSAL_236                      (RESORCE_ID_OSAL +  236U)
#define    OSAL_COND_OSAL_237                      (RESORCE_ID_OSAL +  237U)
#define    OSAL_COND_OSAL_238                      (RESORCE_ID_OSAL +  238U)
#define    OSAL_COND_OSAL_239                      (RESORCE_ID_OSAL +  239U)
#define    OSAL_COND_OSAL_240                      (RESORCE_ID_OSAL +  240U)
#define    OSAL_COND_OSAL_241                      (RESORCE_ID_OSAL +  241U)
#define    OSAL_COND_OSAL_242                      (RESORCE_ID_OSAL +  242U)
#define    OSAL_COND_OSAL_243                      (RESORCE_ID_OSAL +  243U)
#define    OSAL_COND_OSAL_244                      (RESORCE_ID_OSAL +  244U)
#define    OSAL_COND_OSAL_245                      (RESORCE_ID_OSAL +  245U)
#define    OSAL_COND_OSAL_246                      (RESORCE_ID_OSAL +  246U)
#define    OSAL_COND_OSAL_247                      (RESORCE_ID_OSAL +  247U)
#define    OSAL_COND_OSAL_248                      (RESORCE_ID_OSAL +  248U)
#define    OSAL_COND_OSAL_249                      (RESORCE_ID_OSAL +  249U)
#define    OSAL_COND_OSAL_250                      (RESORCE_ID_OSAL +  250U)
#define    OSAL_COND_OSAL_251                      (RESORCE_ID_OSAL +  251U)
#define    OSAL_COND_OSAL_252                      (RESORCE_ID_OSAL +  252U)
#define    OSAL_COND_OSAL_253                      (RESORCE_ID_OSAL +  253U)
#define    OSAL_COND_OSAL_254                      (RESORCE_ID_OSAL +  254U)
#define    OSAL_COND_OSAL_255                      (RESORCE_ID_OSAL +  255U)
#define    OSAL_COND_OSAL_256                      (RESORCE_ID_OSAL +  256U)
#define    OSAL_COND_OSAL_257                      (RESORCE_ID_OSAL +  257U)
#define    OSAL_COND_OSAL_258                      (RESORCE_ID_OSAL +  258U)
#define    OSAL_COND_OSAL_259                      (RESORCE_ID_OSAL +  259U)
#define    OSAL_COND_OSAL_260                      (RESORCE_ID_OSAL +  260U)
#define    OSAL_COND_OSAL_261                      (RESORCE_ID_OSAL +  261U)
#define    OSAL_COND_OSAL_262                      (RESORCE_ID_OSAL +  262U)
#define    OSAL_COND_OSAL_263                      (RESORCE_ID_OSAL +  263U)
#define    OSAL_COND_OSAL_264                      (RESORCE_ID_OSAL +  264U)
#define    OSAL_COND_OSAL_265                      (RESORCE_ID_OSAL +  265U)
#define    OSAL_COND_OSAL_266                      (RESORCE_ID_OSAL +  266U)
#define    OSAL_COND_OSAL_267                      (RESORCE_ID_OSAL +  267U)
#define    OSAL_COND_OSAL_268                      (RESORCE_ID_OSAL +  268U)
#define    OSAL_COND_OSAL_269                      (RESORCE_ID_OSAL +  269U)
#define    OSAL_COND_OSAL_270                      (RESORCE_ID_OSAL +  270U)
#define    OSAL_COND_OSAL_271                      (RESORCE_ID_OSAL +  271U)
#define    OSAL_COND_OSAL_272                      (RESORCE_ID_OSAL +  272U)
#define    OSAL_COND_OSAL_273                      (RESORCE_ID_OSAL +  273U)
#define    OSAL_COND_OSAL_274                      (RESORCE_ID_OSAL +  274U)
#define    OSAL_COND_OSAL_275                      (RESORCE_ID_OSAL +  275U)
#define    OSAL_COND_OSAL_276                      (RESORCE_ID_OSAL +  276U)
#define    OSAL_COND_OSAL_277                      (RESORCE_ID_OSAL +  277U)
#define    OSAL_COND_OSAL_278                      (RESORCE_ID_OSAL +  278U)
#define    OSAL_COND_OSAL_279                      (RESORCE_ID_OSAL +  279U)
#define    OSAL_COND_OSAL_280                      (RESORCE_ID_OSAL +  280U)
#define    OSAL_COND_OSAL_281                      (RESORCE_ID_OSAL +  281U)
#define    OSAL_COND_OSAL_282                      (RESORCE_ID_OSAL +  282U)
#define    OSAL_COND_OSAL_283                      (RESORCE_ID_OSAL +  283U)
#define    OSAL_COND_OSAL_284                      (RESORCE_ID_OSAL +  284U)
#define    OSAL_COND_OSAL_285                      (RESORCE_ID_OSAL +  285U)
#define    OSAL_COND_OSAL_286                      (RESORCE_ID_OSAL +  286U)
#define    OSAL_COND_OSAL_287                      (RESORCE_ID_OSAL +  287U)
#define    OSAL_COND_OSAL_288                      (RESORCE_ID_OSAL +  288U)
#define    OSAL_COND_OSAL_289                      (RESORCE_ID_OSAL +  289U)
#define    OSAL_COND_OSAL_290                      (RESORCE_ID_OSAL +  290U)
#define    OSAL_COND_OSAL_291                      (RESORCE_ID_OSAL +  291U)
#define    OSAL_COND_OSAL_292                      (RESORCE_ID_OSAL +  292U)
#define    OSAL_COND_OSAL_293                      (RESORCE_ID_OSAL +  293U)
#define    OSAL_COND_OSAL_294                      (RESORCE_ID_OSAL +  294U)
#define    OSAL_COND_OSAL_295                      (RESORCE_ID_OSAL +  295U)
#define    OSAL_COND_OSAL_296                      (RESORCE_ID_OSAL +  296U)
#define    OSAL_COND_OSAL_297                      (RESORCE_ID_OSAL +  297U)
#define    OSAL_COND_OSAL_298                      (RESORCE_ID_OSAL +  298U)
#define    OSAL_COND_OSAL_299                      (RESORCE_ID_OSAL +  299U)
#define    OSAL_COND_OSAL_300                      (RESORCE_ID_OSAL +  300U)
#define    OSAL_COND_OSAL_301                      (RESORCE_ID_OSAL +  301U)
#define    OSAL_COND_OSAL_302                      (RESORCE_ID_OSAL +  302U)
#define    OSAL_COND_OSAL_303                      (RESORCE_ID_OSAL +  303U)
#define    OSAL_COND_OSAL_304                      (RESORCE_ID_OSAL +  304U)
#define    OSAL_COND_OSAL_305                      (RESORCE_ID_OSAL +  305U)
#define    OSAL_COND_OSAL_306                      (RESORCE_ID_OSAL +  306U)
#define    OSAL_COND_OSAL_307                      (RESORCE_ID_OSAL +  307U)
#define    OSAL_COND_OSAL_308                      (RESORCE_ID_OSAL +  308U)
#define    OSAL_COND_OSAL_309                      (RESORCE_ID_OSAL +  309U)
#define    OSAL_COND_OSAL_310                      (RESORCE_ID_OSAL +  310U)
#define    OSAL_COND_OSAL_311                      (RESORCE_ID_OSAL +  311U)
#define    OSAL_COND_OSAL_312                      (RESORCE_ID_OSAL +  312U)
#define    OSAL_COND_OSAL_313                      (RESORCE_ID_OSAL +  313U)
#define    OSAL_COND_OSAL_314                      (RESORCE_ID_OSAL +  314U)
#define    OSAL_COND_OSAL_315                      (RESORCE_ID_OSAL +  315U)
#define    OSAL_COND_OSAL_316                      (RESORCE_ID_OSAL +  316U)
#define    OSAL_COND_OSAL_317                      (RESORCE_ID_OSAL +  317U)
#define    OSAL_COND_OSAL_318                      (RESORCE_ID_OSAL +  318U)
#define    OSAL_COND_OSAL_319                      (RESORCE_ID_OSAL +  319U)
#define    OSAL_COND_OSAL_320                      (RESORCE_ID_OSAL +  320U)
#define    OSAL_COND_OSAL_321                      (RESORCE_ID_OSAL +  321U)
#define    OSAL_COND_OSAL_322                      (RESORCE_ID_OSAL +  322U)
#define    OSAL_COND_OSAL_323                      (RESORCE_ID_OSAL +  323U)
#define    OSAL_COND_OSAL_324                      (RESORCE_ID_OSAL +  324U)
#define    OSAL_COND_OSAL_325                      (RESORCE_ID_OSAL +  325U)
#define    OSAL_COND_OSAL_326                      (RESORCE_ID_OSAL +  326U)
#define    OSAL_COND_OSAL_327                      (RESORCE_ID_OSAL +  327U)
#define    OSAL_COND_OSAL_328                      (RESORCE_ID_OSAL +  328U)
#define    OSAL_COND_OSAL_329                      (RESORCE_ID_OSAL +  329U)
#define    OSAL_COND_OSAL_330                      (RESORCE_ID_OSAL +  330U)
#define    OSAL_COND_OSAL_331                      (RESORCE_ID_OSAL +  331U)
#define    OSAL_COND_OSAL_332                      (RESORCE_ID_OSAL +  332U)
#define    OSAL_COND_OSAL_333                      (RESORCE_ID_OSAL +  333U)
#define    OSAL_COND_OSAL_334                      (RESORCE_ID_OSAL +  334U)
#define    OSAL_COND_OSAL_335                      (RESORCE_ID_OSAL +  335U)
#define    OSAL_COND_OSAL_336                      (RESORCE_ID_OSAL +  336U)
#define    OSAL_COND_OSAL_337                      (RESORCE_ID_OSAL +  337U)
#define    OSAL_COND_OSAL_338                      (RESORCE_ID_OSAL +  338U)
#define    OSAL_COND_OSAL_339                      (RESORCE_ID_OSAL +  339U)
#define    OSAL_COND_OSAL_340                      (RESORCE_ID_OSAL +  340U)
#define    OSAL_COND_OSAL_341                      (RESORCE_ID_OSAL +  341U)
#define    OSAL_COND_OSAL_342                      (RESORCE_ID_OSAL +  342U)
#define    OSAL_COND_OSAL_343                      (RESORCE_ID_OSAL +  343U)
#define    OSAL_COND_OSAL_344                      (RESORCE_ID_OSAL +  344U)
#define    OSAL_COND_OSAL_345                      (RESORCE_ID_OSAL +  345U)
#define    OSAL_COND_OSAL_346                      (RESORCE_ID_OSAL +  346U)
#define    OSAL_COND_OSAL_347                      (RESORCE_ID_OSAL +  347U)
#define    OSAL_COND_OSAL_348                      (RESORCE_ID_OSAL +  348U)
#define    OSAL_COND_OSAL_349                      (RESORCE_ID_OSAL +  349U)
#define    OSAL_COND_OSAL_350                      (RESORCE_ID_OSAL +  350U)
#define    OSAL_COND_OSAL_351                      (RESORCE_ID_OSAL +  351U)
#define    OSAL_COND_OSAL_352                      (RESORCE_ID_OSAL +  352U)
#define    OSAL_COND_OSAL_353                      (RESORCE_ID_OSAL +  353U)
#define    OSAL_COND_OSAL_354                      (RESORCE_ID_OSAL +  354U)
#define    OSAL_COND_OSAL_355                      (RESORCE_ID_OSAL +  355U)
#define    OSAL_COND_OSAL_356                      (RESORCE_ID_OSAL +  356U)
#define    OSAL_COND_OSAL_357                      (RESORCE_ID_OSAL +  357U)
#define    OSAL_COND_OSAL_358                      (RESORCE_ID_OSAL +  358U)
#define    OSAL_COND_OSAL_359                      (RESORCE_ID_OSAL +  359U)
#define    OSAL_COND_OSAL_360                      (RESORCE_ID_OSAL +  360U)
#define    OSAL_COND_OSAL_361                      (RESORCE_ID_OSAL +  361U)
#define    OSAL_COND_OSAL_362                      (RESORCE_ID_OSAL +  362U)
#define    OSAL_COND_OSAL_363                      (RESORCE_ID_OSAL +  363U)
#define    OSAL_COND_OSAL_364                      (RESORCE_ID_OSAL +  364U)
#define    OSAL_COND_OSAL_365                      (RESORCE_ID_OSAL +  365U)
#define    OSAL_COND_OSAL_366                      (RESORCE_ID_OSAL +  366U)
#define    OSAL_COND_OSAL_367                      (RESORCE_ID_OSAL +  367U)
#define    OSAL_COND_OSAL_368                      (RESORCE_ID_OSAL +  368U)
#define    OSAL_COND_OSAL_369                      (RESORCE_ID_OSAL +  369U)
#define    OSAL_COND_OSAL_370                      (RESORCE_ID_OSAL +  370U)
#define    OSAL_COND_OSAL_371                      (RESORCE_ID_OSAL +  371U)
#define    OSAL_COND_OSAL_372                      (RESORCE_ID_OSAL +  372U)
#define    OSAL_COND_OSAL_373                      (RESORCE_ID_OSAL +  373U)
#define    OSAL_COND_OSAL_374                      (RESORCE_ID_OSAL +  374U)
#define    OSAL_COND_OSAL_375                      (RESORCE_ID_OSAL +  375U)
#define    OSAL_COND_OSAL_376                      (RESORCE_ID_OSAL +  376U)
#define    OSAL_COND_OSAL_377                      (RESORCE_ID_OSAL +  377U)
#define    OSAL_COND_OSAL_378                      (RESORCE_ID_OSAL +  378U)
#define    OSAL_COND_OSAL_379                      (RESORCE_ID_OSAL +  379U)
#define    OSAL_COND_OSAL_380                      (RESORCE_ID_OSAL +  380U)
#define    OSAL_COND_OSAL_381                      (RESORCE_ID_OSAL +  381U)
#define    OSAL_COND_OSAL_382                      (RESORCE_ID_OSAL +  382U)
#define    OSAL_COND_OSAL_383                      (RESORCE_ID_OSAL +  383U)
#define    OSAL_COND_OSAL_384                      (RESORCE_ID_OSAL +  384U)
#define    OSAL_COND_OSAL_385                      (RESORCE_ID_OSAL +  385U)
#define    OSAL_COND_OSAL_386                      (RESORCE_ID_OSAL +  386U)
#define    OSAL_COND_OSAL_387                      (RESORCE_ID_OSAL +  387U)
#define    OSAL_COND_OSAL_388                      (RESORCE_ID_OSAL +  388U)
#define    OSAL_COND_OSAL_389                      (RESORCE_ID_OSAL +  389U)
#define    OSAL_COND_OSAL_390                      (RESORCE_ID_OSAL +  390U)
#define    OSAL_COND_OSAL_391                      (RESORCE_ID_OSAL +  391U)
#define    OSAL_COND_OSAL_392                      (RESORCE_ID_OSAL +  392U)
#define    OSAL_COND_OSAL_393                      (RESORCE_ID_OSAL +  393U)
#define    OSAL_COND_OSAL_394                      (RESORCE_ID_OSAL +  394U)
#define    OSAL_COND_OSAL_395                      (RESORCE_ID_OSAL +  395U)
#define    OSAL_COND_OSAL_396                      (RESORCE_ID_OSAL +  396U)
#define    OSAL_COND_OSAL_397                      (RESORCE_ID_OSAL +  397U)
#define    OSAL_COND_OSAL_398                      (RESORCE_ID_OSAL +  398U)
#define    OSAL_COND_OSAL_399                      (RESORCE_ID_OSAL +  399U)
#define    OSAL_COND_OSAL_400                      (RESORCE_ID_OSAL +  400U)
#define    OSAL_COND_OSAL_401                      (RESORCE_ID_OSAL +  401U)
#define    OSAL_COND_OSAL_402                      (RESORCE_ID_OSAL +  402U)
#define    OSAL_COND_OSAL_403                      (RESORCE_ID_OSAL +  403U)
#define    OSAL_COND_OSAL_404                      (RESORCE_ID_OSAL +  404U)
#define    OSAL_COND_OSAL_405                      (RESORCE_ID_OSAL +  405U)
#define    OSAL_COND_OSAL_406                      (RESORCE_ID_OSAL +  406U)
#define    OSAL_COND_OSAL_407                      (RESORCE_ID_OSAL +  407U)
#define    OSAL_COND_OSAL_408                      (RESORCE_ID_OSAL +  408U)
#define    OSAL_COND_OSAL_409                      (RESORCE_ID_OSAL +  409U)
#define    OSAL_COND_OSAL_410                      (RESORCE_ID_OSAL +  410U)
#define    OSAL_COND_OSAL_411                      (RESORCE_ID_OSAL +  411U)
#define    OSAL_COND_OSAL_412                      (RESORCE_ID_OSAL +  412U)
#define    OSAL_COND_OSAL_413                      (RESORCE_ID_OSAL +  413U)
#define    OSAL_COND_OSAL_414                      (RESORCE_ID_OSAL +  414U)
#define    OSAL_COND_OSAL_415                      (RESORCE_ID_OSAL +  415U)
#define    OSAL_COND_OSAL_416                      (RESORCE_ID_OSAL +  416U)
#define    OSAL_COND_OSAL_417                      (RESORCE_ID_OSAL +  417U)
#define    OSAL_COND_OSAL_418                      (RESORCE_ID_OSAL +  418U)
#define    OSAL_COND_OSAL_419                      (RESORCE_ID_OSAL +  419U)
#define    OSAL_COND_OSAL_420                      (RESORCE_ID_OSAL +  420U)
#define    OSAL_COND_OSAL_421                      (RESORCE_ID_OSAL +  421U)
#define    OSAL_COND_OSAL_422                      (RESORCE_ID_OSAL +  422U)
#define    OSAL_COND_OSAL_423                      (RESORCE_ID_OSAL +  423U)
#define    OSAL_COND_OSAL_424                      (RESORCE_ID_OSAL +  424U)
#define    OSAL_COND_OSAL_425                      (RESORCE_ID_OSAL +  425U)
#define    OSAL_COND_OSAL_426                      (RESORCE_ID_OSAL +  426U)
#define    OSAL_COND_OSAL_427                      (RESORCE_ID_OSAL +  427U)
#define    OSAL_COND_OSAL_428                      (RESORCE_ID_OSAL +  428U)
#define    OSAL_COND_OSAL_429                      (RESORCE_ID_OSAL +  429U)
#define    OSAL_COND_OSAL_430                      (RESORCE_ID_OSAL +  430U)
#define    OSAL_COND_OSAL_431                      (RESORCE_ID_OSAL +  431U)
#define    OSAL_COND_OSAL_432                      (RESORCE_ID_OSAL +  432U)
#define    OSAL_COND_OSAL_433                      (RESORCE_ID_OSAL +  433U)
#define    OSAL_COND_OSAL_434                      (RESORCE_ID_OSAL +  434U)
#define    OSAL_COND_OSAL_435                      (RESORCE_ID_OSAL +  435U)
#define    OSAL_COND_OSAL_436                      (RESORCE_ID_OSAL +  436U)
#define    OSAL_COND_OSAL_437                      (RESORCE_ID_OSAL +  437U)
#define    OSAL_COND_OSAL_438                      (RESORCE_ID_OSAL +  438U)
#define    OSAL_COND_OSAL_439                      (RESORCE_ID_OSAL +  439U)
#define    OSAL_COND_OSAL_440                      (RESORCE_ID_OSAL +  440U)
#define    OSAL_COND_OSAL_441                      (RESORCE_ID_OSAL +  441U)
#define    OSAL_COND_OSAL_442                      (RESORCE_ID_OSAL +  442U)
#define    OSAL_COND_OSAL_443                      (RESORCE_ID_OSAL +  443U)
#define    OSAL_COND_OSAL_444                      (RESORCE_ID_OSAL +  444U)
#define    OSAL_COND_OSAL_445                      (RESORCE_ID_OSAL +  445U)
#define    OSAL_COND_OSAL_446                      (RESORCE_ID_OSAL +  446U)
#define    OSAL_COND_OSAL_447                      (RESORCE_ID_OSAL +  447U)
#define    OSAL_COND_OSAL_448                      (RESORCE_ID_OSAL +  448U)
#define    OSAL_COND_OSAL_449                      (RESORCE_ID_OSAL +  449U)
#define    OSAL_COND_OSAL_450                      (RESORCE_ID_OSAL +  450U)
#define    OSAL_COND_OSAL_451                      (RESORCE_ID_OSAL +  451U)
#define    OSAL_COND_OSAL_452                      (RESORCE_ID_OSAL +  452U)
#define    OSAL_COND_OSAL_453                      (RESORCE_ID_OSAL +  453U)
#define    OSAL_COND_OSAL_454                      (RESORCE_ID_OSAL +  454U)
#define    OSAL_COND_OSAL_455                      (RESORCE_ID_OSAL +  455U)
#define    OSAL_COND_OSAL_456                      (RESORCE_ID_OSAL +  456U)
#define    OSAL_COND_OSAL_457                      (RESORCE_ID_OSAL +  457U)
#define    OSAL_COND_OSAL_458                      (RESORCE_ID_OSAL +  458U)
#define    OSAL_COND_OSAL_459                      (RESORCE_ID_OSAL +  459U)
#define    OSAL_COND_OSAL_460                      (RESORCE_ID_OSAL +  460U)
#define    OSAL_COND_OSAL_461                      (RESORCE_ID_OSAL +  461U)
#define    OSAL_COND_OSAL_462                      (RESORCE_ID_OSAL +  462U)
#define    OSAL_COND_OSAL_463                      (RESORCE_ID_OSAL +  463U)
#define    OSAL_COND_OSAL_464                      (RESORCE_ID_OSAL +  464U)
#define    OSAL_COND_OSAL_465                      (RESORCE_ID_OSAL +  465U)
#define    OSAL_COND_OSAL_466                      (RESORCE_ID_OSAL +  466U)
#define    OSAL_COND_OSAL_467                      (RESORCE_ID_OSAL +  467U)
#define    OSAL_COND_OSAL_468                      (RESORCE_ID_OSAL +  468U)
#define    OSAL_COND_OSAL_469                      (RESORCE_ID_OSAL +  469U)
#define    OSAL_COND_OSAL_470                      (RESORCE_ID_OSAL +  470U)
#define    OSAL_COND_OSAL_471                      (RESORCE_ID_OSAL +  471U)
#define    OSAL_COND_OSAL_472                      (RESORCE_ID_OSAL +  472U)
#define    OSAL_COND_OSAL_473                      (RESORCE_ID_OSAL +  473U)
#define    OSAL_COND_OSAL_474                      (RESORCE_ID_OSAL +  474U)
#define    OSAL_COND_OSAL_475                      (RESORCE_ID_OSAL +  475U)
#define    OSAL_COND_OSAL_476                      (RESORCE_ID_OSAL +  476U)
#define    OSAL_COND_OSAL_477                      (RESORCE_ID_OSAL +  477U)
#define    OSAL_COND_OSAL_478                      (RESORCE_ID_OSAL +  478U)
#define    OSAL_COND_OSAL_479                      (RESORCE_ID_OSAL +  479U)
#define    OSAL_COND_OSAL_480                      (RESORCE_ID_OSAL +  480U)
#define    OSAL_COND_OSAL_481                      (RESORCE_ID_OSAL +  481U)
#define    OSAL_COND_OSAL_482                      (RESORCE_ID_OSAL +  482U)
#define    OSAL_COND_OSAL_483                      (RESORCE_ID_OSAL +  483U)
#define    OSAL_COND_OSAL_484                      (RESORCE_ID_OSAL +  484U)
#define    OSAL_COND_OSAL_485                      (RESORCE_ID_OSAL +  485U)
#define    OSAL_COND_OSAL_486                      (RESORCE_ID_OSAL +  486U)
#define    OSAL_COND_OSAL_487                      (RESORCE_ID_OSAL +  487U)
#define    OSAL_COND_OSAL_488                      (RESORCE_ID_OSAL +  488U)
#define    OSAL_COND_OSAL_489                      (RESORCE_ID_OSAL +  489U)
#define    OSAL_COND_OSAL_490                      (RESORCE_ID_OSAL +  490U)
#define    OSAL_COND_OSAL_491                      (RESORCE_ID_OSAL +  491U)
#define    OSAL_COND_OSAL_492                      (RESORCE_ID_OSAL +  492U)
#define    OSAL_COND_OSAL_493                      (RESORCE_ID_OSAL +  493U)
#define    OSAL_COND_OSAL_494                      (RESORCE_ID_OSAL +  494U)
#define    OSAL_COND_OSAL_495                      (RESORCE_ID_OSAL +  495U)
#define    OSAL_COND_OSAL_496                      (RESORCE_ID_OSAL +  496U)
#define    OSAL_COND_OSAL_497                      (RESORCE_ID_OSAL +  497U)
#define    OSAL_COND_OSAL_498                      (RESORCE_ID_OSAL +  498U)
#define    OSAL_COND_OSAL_499                      (RESORCE_ID_OSAL +  499U)
#define    OSAL_COND_OSAL_500                      (RESORCE_ID_OSAL +  500U)

/*******************************************************************************************************************//**
 * @def OSAL_MQ_ID
 * Message queue IDs.
***********************************************************************************************************************/
/* xOS2 resource information */
#define    OSAL_MQ_OSAL_00                           (RESORCE_ID_OSAL  +  0U)
#define    OSAL_MQ_OSAL_01                           (RESORCE_ID_OSAL  +  1U)
#define    OSAL_MQ_OSAL_02                           (RESORCE_ID_OSAL  +  2U)
#define    OSAL_MQ_OSAL_03                           (RESORCE_ID_OSAL  +  3U)
#define    OSAL_MQ_OSAL_04                           (RESORCE_ID_OSAL  +  4U)
#define    OSAL_MQ_OSAL_05                           (RESORCE_ID_OSAL  +  5U)
#define    OSAL_MQ_OSAL_06                           (RESORCE_ID_OSAL  +  6U)
#define    OSAL_MQ_OSAL_07                           (RESORCE_ID_OSAL  +  7U)
#define    OSAL_MQ_OSAL_08                           (RESORCE_ID_OSAL  +  8U)
#define    OSAL_MQ_OSAL_09                           (RESORCE_ID_OSAL  +  9U)
#define    OSAL_MQ_OSAL_10                           (RESORCE_ID_OSAL  +  10U)
#define    OSAL_MQ_OSAL_11                           (RESORCE_ID_OSAL  +  11U)
#define    OSAL_MQ_OSAL_12                           (RESORCE_ID_OSAL  +  12U)
#define    OSAL_MQ_OSAL_13                           (RESORCE_ID_OSAL  +  13U)
#define    OSAL_MQ_OSAL_14                           (RESORCE_ID_OSAL  +  14U)
#define    OSAL_MQ_OSAL_15                           (RESORCE_ID_OSAL  +  15U)
#define    OSAL_MQ_OSAL_16                           (RESORCE_ID_OSAL  +  16U)
#define    OSAL_MQ_OSAL_17                           (RESORCE_ID_OSAL  +  17U)
#define    OSAL_MQ_OSAL_18                           (RESORCE_ID_OSAL  +  18U)
#define    OSAL_MQ_OSAL_19                           (RESORCE_ID_OSAL  +  19U)
#define    OSAL_MQ_OSAL_20                           (RESORCE_ID_OSAL  +  20U)
#define    OSAL_MQ_OSAL_21                           (RESORCE_ID_OSAL  +  21U)
#define    OSAL_MQ_OSAL_22                           (RESORCE_ID_OSAL  +  22U)
#define    OSAL_MQ_OSAL_23                           (RESORCE_ID_OSAL  +  23U)
#define    OSAL_MQ_OSAL_24                           (RESORCE_ID_OSAL  +  24U)
#define    OSAL_MQ_OSAL_25                           (RESORCE_ID_OSAL  +  25U)
#define    OSAL_MQ_OSAL_26                           (RESORCE_ID_OSAL  +  26U)
#define    OSAL_MQ_OSAL_27                           (RESORCE_ID_OSAL  +  27U)
#define    OSAL_MQ_OSAL_28                           (RESORCE_ID_OSAL  +  28U)
#define    OSAL_MQ_OSAL_29                           (RESORCE_ID_OSAL  +  29U)
#define    OSAL_MQ_OSAL_30                           (RESORCE_ID_OSAL  +  30U)
#define    OSAL_MQ_OSAL_31                           (RESORCE_ID_OSAL  +  31U)
#define    OSAL_MQ_OSAL_32                           (RESORCE_ID_OSAL  +  32U)
#define    OSAL_MQ_OSAL_33                           (RESORCE_ID_OSAL  +  33U)
#define    OSAL_MQ_OSAL_34                           (RESORCE_ID_OSAL  +  34U)
#define    OSAL_MQ_OSAL_35                           (RESORCE_ID_OSAL  +  35U)
#define    OSAL_MQ_OSAL_36                           (RESORCE_ID_OSAL  +  36U)
#define    OSAL_MQ_OSAL_37                           (RESORCE_ID_OSAL  +  37U)
#define    OSAL_MQ_OSAL_38                           (RESORCE_ID_OSAL  +  38U)
#define    OSAL_MQ_OSAL_39                           (RESORCE_ID_OSAL  +  39U)
#define    OSAL_MQ_OSAL_40                           (RESORCE_ID_OSAL  +  40U)
#define    OSAL_MQ_OSAL_41                           (RESORCE_ID_OSAL  +  41U)
#define    OSAL_MQ_OSAL_42                           (RESORCE_ID_OSAL  +  42U)
#define    OSAL_MQ_OSAL_43                           (RESORCE_ID_OSAL  +  43U)
#define    OSAL_MQ_OSAL_44                           (RESORCE_ID_OSAL  +  44U)
#define    OSAL_MQ_OSAL_45                           (RESORCE_ID_OSAL  +  45U)
#define    OSAL_MQ_OSAL_46                           (RESORCE_ID_OSAL  +  46U)
#define    OSAL_MQ_OSAL_47                           (RESORCE_ID_OSAL  +  47U)
#define    OSAL_MQ_OSAL_48                           (RESORCE_ID_OSAL  +  48U)
#define    OSAL_MQ_OSAL_49                           (RESORCE_ID_OSAL  +  49U)
#define    OSAL_MQ_OSAL_50                           (RESORCE_ID_OSAL  +  50U)
#define    OSAL_MQ_OSAL_51                           (RESORCE_ID_OSAL  +  51U)
#define    OSAL_MQ_OSAL_52                           (RESORCE_ID_OSAL  +  52U)
#define    OSAL_MQ_OSAL_53                           (RESORCE_ID_OSAL  +  53U)
#define    OSAL_MQ_OSAL_54                           (RESORCE_ID_OSAL  +  54U)
#define    OSAL_MQ_OSAL_55                           (RESORCE_ID_OSAL  +  55U)
#define    OSAL_MQ_OSAL_56                           (RESORCE_ID_OSAL  +  56U)
#define    OSAL_MQ_OSAL_57                           (RESORCE_ID_OSAL  +  57U)
#define    OSAL_MQ_OSAL_58                           (RESORCE_ID_OSAL  +  58U)
#define    OSAL_MQ_OSAL_59                           (RESORCE_ID_OSAL  +  59U)
#define    OSAL_MQ_OSAL_60                           (RESORCE_ID_OSAL  +  60U)
#define    OSAL_MQ_OSAL_61                           (RESORCE_ID_OSAL  +  61U)
#define    OSAL_MQ_OSAL_62                           (RESORCE_ID_OSAL  +  62U)
#define    OSAL_MQ_OSAL_63                           (RESORCE_ID_OSAL  +  63U)
#define    OSAL_MQ_OSAL_64                           (RESORCE_ID_OSAL  +  64U)
#define    OSAL_MQ_OSAL_65                           (RESORCE_ID_OSAL  +  65U)
#define    OSAL_MQ_OSAL_66                           (RESORCE_ID_OSAL  +  66U)
#define    OSAL_MQ_OSAL_67                           (RESORCE_ID_OSAL  +  67U)
#define    OSAL_MQ_OSAL_68                           (RESORCE_ID_OSAL  +  68U)
#define    OSAL_MQ_OSAL_69                           (RESORCE_ID_OSAL  +  69U)
#define    OSAL_MQ_OSAL_70                           (RESORCE_ID_OSAL  +  70U)
#define    OSAL_MQ_OSAL_71                           (RESORCE_ID_OSAL  +  71U)
#define    OSAL_MQ_OSAL_72                           (RESORCE_ID_OSAL  +  72U)
#define    OSAL_MQ_OSAL_73                           (RESORCE_ID_OSAL  +  73U)
#define    OSAL_MQ_OSAL_74                           (RESORCE_ID_OSAL  +  74U)
#define    OSAL_MQ_OSAL_75                           (RESORCE_ID_OSAL  +  75U)
#define    OSAL_MQ_OSAL_76                           (RESORCE_ID_OSAL  +  76U)
#define    OSAL_MQ_OSAL_77                           (RESORCE_ID_OSAL  +  77U)
#define    OSAL_MQ_OSAL_78                           (RESORCE_ID_OSAL  +  78U)
#define    OSAL_MQ_OSAL_79                           (RESORCE_ID_OSAL  +  79U)
#define    OSAL_MQ_OSAL_80                           (RESORCE_ID_OSAL  +  80U)
#define    OSAL_MQ_OSAL_81                           (RESORCE_ID_OSAL  +  81U)
#define    OSAL_MQ_OSAL_82                           (RESORCE_ID_OSAL  +  82U)
#define    OSAL_MQ_OSAL_83                           (RESORCE_ID_OSAL  +  83U)
#define    OSAL_MQ_OSAL_84                           (RESORCE_ID_OSAL  +  84U)
#define    OSAL_MQ_OSAL_85                           (RESORCE_ID_OSAL  +  85U)
#define    OSAL_MQ_OSAL_86                           (RESORCE_ID_OSAL  +  86U)
#define    OSAL_MQ_OSAL_87                           (RESORCE_ID_OSAL  +  87U)
#define    OSAL_MQ_OSAL_88                           (RESORCE_ID_OSAL  +  88U)
#define    OSAL_MQ_OSAL_89                           (RESORCE_ID_OSAL  +  89U)
#define    OSAL_MQ_OSAL_90                           (RESORCE_ID_OSAL  +  90U)
#define    OSAL_MQ_OSAL_91                           (RESORCE_ID_OSAL  +  91U)
#define    OSAL_MQ_OSAL_92                           (RESORCE_ID_OSAL  +  92U)
#define    OSAL_MQ_OSAL_93                           (RESORCE_ID_OSAL  +  93U)
#define    OSAL_MQ_OSAL_94                           (RESORCE_ID_OSAL  +  94U)
#define    OSAL_MQ_OSAL_95                           (RESORCE_ID_OSAL  +  95U)
#define    OSAL_MQ_OSAL_96                           (RESORCE_ID_OSAL  +  96U)
#define    OSAL_MQ_OSAL_97                           (RESORCE_ID_OSAL  +  97U)
#define    OSAL_MQ_OSAL_98                           (RESORCE_ID_OSAL  +  98U)
#define    OSAL_MQ_OSAL_99                           (RESORCE_ID_OSAL  +  99U)
#define    OSAL_MQ_OSAL_100                           (RESORCE_ID_OSAL  +  100U)
#define    OSAL_MQ_OSAL_101                           (RESORCE_ID_OSAL  +  101U)
#define    OSAL_MQ_OSAL_102                           (RESORCE_ID_OSAL  +  102U)
#define    OSAL_MQ_OSAL_103                           (RESORCE_ID_OSAL  +  103U)
#define    OSAL_MQ_OSAL_104                           (RESORCE_ID_OSAL  +  104U)
#define    OSAL_MQ_OSAL_105                           (RESORCE_ID_OSAL  +  105U)
#define    OSAL_MQ_OSAL_106                           (RESORCE_ID_OSAL  +  106U)
#define    OSAL_MQ_OSAL_107                           (RESORCE_ID_OSAL  +  107U)
#define    OSAL_MQ_OSAL_108                           (RESORCE_ID_OSAL  +  108U)
#define    OSAL_MQ_OSAL_109                           (RESORCE_ID_OSAL  +  109U)
#define    OSAL_MQ_OSAL_110                           (RESORCE_ID_OSAL  +  110U)
#define    OSAL_MQ_OSAL_111                           (RESORCE_ID_OSAL  +  111U)
#define    OSAL_MQ_OSAL_112                           (RESORCE_ID_OSAL  +  112U)
#define    OSAL_MQ_OSAL_113                           (RESORCE_ID_OSAL  +  113U)
#define    OSAL_MQ_OSAL_114                           (RESORCE_ID_OSAL  +  114U)
#define    OSAL_MQ_OSAL_115                           (RESORCE_ID_OSAL  +  115U)
#define    OSAL_MQ_OSAL_116                           (RESORCE_ID_OSAL  +  116U)
#define    OSAL_MQ_OSAL_117                           (RESORCE_ID_OSAL  +  117U)
#define    OSAL_MQ_OSAL_118                           (RESORCE_ID_OSAL  +  118U)
#define    OSAL_MQ_OSAL_119                           (RESORCE_ID_OSAL  +  119U)
#define    OSAL_MQ_OSAL_120                           (RESORCE_ID_OSAL  +  120U)
#define    OSAL_MQ_OSAL_121                           (RESORCE_ID_OSAL  +  121U)
#define    OSAL_MQ_OSAL_122                           (RESORCE_ID_OSAL  +  122U)
#define    OSAL_MQ_OSAL_123                           (RESORCE_ID_OSAL  +  123U)
#define    OSAL_MQ_OSAL_124                           (RESORCE_ID_OSAL  +  124U)
#define    OSAL_MQ_OSAL_125                           (RESORCE_ID_OSAL  +  125U)
#define    OSAL_MQ_OSAL_126                           (RESORCE_ID_OSAL  +  126U)
#define    OSAL_MQ_OSAL_127                           (RESORCE_ID_OSAL  +  127U)
#define    OSAL_MQ_OSAL_128                           (RESORCE_ID_OSAL  +  128U)
#define    OSAL_MQ_OSAL_129                           (RESORCE_ID_OSAL  +  129U)
#define    OSAL_MQ_OSAL_130                           (RESORCE_ID_OSAL  +  130U)
#define    OSAL_MQ_OSAL_131                           (RESORCE_ID_OSAL  +  131U)
#define    OSAL_MQ_OSAL_132                           (RESORCE_ID_OSAL  +  132U)
#define    OSAL_MQ_OSAL_133                           (RESORCE_ID_OSAL  +  133U)
#define    OSAL_MQ_OSAL_134                           (RESORCE_ID_OSAL  +  134U)
#define    OSAL_MQ_OSAL_135                           (RESORCE_ID_OSAL  +  135U)
#define    OSAL_MQ_OSAL_136                           (RESORCE_ID_OSAL  +  136U)
#define    OSAL_MQ_OSAL_137                           (RESORCE_ID_OSAL  +  137U)
#define    OSAL_MQ_OSAL_138                           (RESORCE_ID_OSAL  +  138U)
#define    OSAL_MQ_OSAL_139                           (RESORCE_ID_OSAL  +  139U)
#define    OSAL_MQ_OSAL_140                           (RESORCE_ID_OSAL  +  140U)
#define    OSAL_MQ_OSAL_141                           (RESORCE_ID_OSAL  +  141U)
#define    OSAL_MQ_OSAL_142                           (RESORCE_ID_OSAL  +  142U)
#define    OSAL_MQ_OSAL_143                           (RESORCE_ID_OSAL  +  143U)
#define    OSAL_MQ_OSAL_144                           (RESORCE_ID_OSAL  +  144U)
#define    OSAL_MQ_OSAL_145                           (RESORCE_ID_OSAL  +  145U)
#define    OSAL_MQ_OSAL_146                           (RESORCE_ID_OSAL  +  146U)
#define    OSAL_MQ_OSAL_147                           (RESORCE_ID_OSAL  +  147U)
#define    OSAL_MQ_OSAL_148                           (RESORCE_ID_OSAL  +  148U)
#define    OSAL_MQ_OSAL_149                           (RESORCE_ID_OSAL  +  149U)
#define    OSAL_MQ_OSAL_150                           (RESORCE_ID_OSAL  +  150U)
#define    OSAL_MQ_OSAL_151                           (RESORCE_ID_OSAL  +  151U)
#define    OSAL_MQ_OSAL_152                           (RESORCE_ID_OSAL  +  152U)
#define    OSAL_MQ_OSAL_153                           (RESORCE_ID_OSAL  +  153U)
#define    OSAL_MQ_OSAL_154                           (RESORCE_ID_OSAL  +  154U)
#define    OSAL_MQ_OSAL_155                           (RESORCE_ID_OSAL  +  155U)
#define    OSAL_MQ_OSAL_156                           (RESORCE_ID_OSAL  +  156U)
#define    OSAL_MQ_OSAL_157                           (RESORCE_ID_OSAL  +  157U)
#define    OSAL_MQ_OSAL_158                           (RESORCE_ID_OSAL  +  158U)
#define    OSAL_MQ_OSAL_159                           (RESORCE_ID_OSAL  +  159U)
#define    OSAL_MQ_OSAL_160                           (RESORCE_ID_OSAL  +  160U)
#define    OSAL_MQ_OSAL_161                           (RESORCE_ID_OSAL  +  161U)
#define    OSAL_MQ_OSAL_162                           (RESORCE_ID_OSAL  +  162U)
#define    OSAL_MQ_OSAL_163                           (RESORCE_ID_OSAL  +  163U)
#define    OSAL_MQ_OSAL_164                           (RESORCE_ID_OSAL  +  164U)
#define    OSAL_MQ_OSAL_165                           (RESORCE_ID_OSAL  +  165U)
#define    OSAL_MQ_OSAL_166                           (RESORCE_ID_OSAL  +  166U)
#define    OSAL_MQ_OSAL_167                           (RESORCE_ID_OSAL  +  167U)
#define    OSAL_MQ_OSAL_168                           (RESORCE_ID_OSAL  +  168U)
#define    OSAL_MQ_OSAL_169                           (RESORCE_ID_OSAL  +  169U)
#define    OSAL_MQ_OSAL_170                           (RESORCE_ID_OSAL  +  170U)
#define    OSAL_MQ_OSAL_171                           (RESORCE_ID_OSAL  +  171U)
#define    OSAL_MQ_OSAL_172                           (RESORCE_ID_OSAL  +  172U)
#define    OSAL_MQ_OSAL_173                           (RESORCE_ID_OSAL  +  173U)
#define    OSAL_MQ_OSAL_174                           (RESORCE_ID_OSAL  +  174U)
#define    OSAL_MQ_OSAL_175                           (RESORCE_ID_OSAL  +  175U)
#define    OSAL_MQ_OSAL_176                           (RESORCE_ID_OSAL  +  176U)
#define    OSAL_MQ_OSAL_177                           (RESORCE_ID_OSAL  +  177U)
#define    OSAL_MQ_OSAL_178                           (RESORCE_ID_OSAL  +  178U)
#define    OSAL_MQ_OSAL_179                           (RESORCE_ID_OSAL  +  179U)
#define    OSAL_MQ_OSAL_180                           (RESORCE_ID_OSAL  +  180U)
#define    OSAL_MQ_OSAL_181                           (RESORCE_ID_OSAL  +  181U)
#define    OSAL_MQ_OSAL_182                           (RESORCE_ID_OSAL  +  182U)
#define    OSAL_MQ_OSAL_183                           (RESORCE_ID_OSAL  +  183U)
#define    OSAL_MQ_OSAL_184                           (RESORCE_ID_OSAL  +  184U)
#define    OSAL_MQ_OSAL_185                           (RESORCE_ID_OSAL  +  185U)
#define    OSAL_MQ_OSAL_186                           (RESORCE_ID_OSAL  +  186U)
#define    OSAL_MQ_OSAL_187                           (RESORCE_ID_OSAL  +  187U)
#define    OSAL_MQ_OSAL_188                           (RESORCE_ID_OSAL  +  188U)
#define    OSAL_MQ_OSAL_189                           (RESORCE_ID_OSAL  +  189U)
#define    OSAL_MQ_OSAL_190                           (RESORCE_ID_OSAL  +  190U)
#define    OSAL_MQ_OSAL_191                           (RESORCE_ID_OSAL  +  191U)
#define    OSAL_MQ_OSAL_192                           (RESORCE_ID_OSAL  +  192U)
#define    OSAL_MQ_OSAL_193                           (RESORCE_ID_OSAL  +  193U)
#define    OSAL_MQ_OSAL_194                           (RESORCE_ID_OSAL  +  194U)
#define    OSAL_MQ_OSAL_195                           (RESORCE_ID_OSAL  +  195U)
#define    OSAL_MQ_OSAL_196                           (RESORCE_ID_OSAL  +  196U)
#define    OSAL_MQ_OSAL_197                           (RESORCE_ID_OSAL  +  197U)
#define    OSAL_MQ_OSAL_198                           (RESORCE_ID_OSAL  +  198U)
#define    OSAL_MQ_OSAL_199                           (RESORCE_ID_OSAL  +  199U)
#define    OSAL_MQ_OSAL_200                           (RESORCE_ID_OSAL  +  200U)
#define    OSAL_MQ_OSAL_201                           (RESORCE_ID_OSAL  +  201U)
#define    OSAL_MQ_OSAL_202                           (RESORCE_ID_OSAL  +  202U)
#define    OSAL_MQ_OSAL_203                           (RESORCE_ID_OSAL  +  203U)
#define    OSAL_MQ_OSAL_204                           (RESORCE_ID_OSAL  +  204U)
#define    OSAL_MQ_OSAL_205                           (RESORCE_ID_OSAL  +  205U)
#define    OSAL_MQ_OSAL_206                           (RESORCE_ID_OSAL  +  206U)
#define    OSAL_MQ_OSAL_207                           (RESORCE_ID_OSAL  +  207U)
#define    OSAL_MQ_OSAL_208                           (RESORCE_ID_OSAL  +  208U)
#define    OSAL_MQ_OSAL_209                           (RESORCE_ID_OSAL  +  209U)
#define    OSAL_MQ_OSAL_210                           (RESORCE_ID_OSAL  +  210U)
#define    OSAL_MQ_OSAL_211                           (RESORCE_ID_OSAL  +  211U)
#define    OSAL_MQ_OSAL_212                           (RESORCE_ID_OSAL  +  212U)
#define    OSAL_MQ_OSAL_213                           (RESORCE_ID_OSAL  +  213U)
#define    OSAL_MQ_OSAL_214                           (RESORCE_ID_OSAL  +  214U)
#define    OSAL_MQ_OSAL_215                           (RESORCE_ID_OSAL  +  215U)
#define    OSAL_MQ_OSAL_216                           (RESORCE_ID_OSAL  +  216U)
#define    OSAL_MQ_OSAL_217                           (RESORCE_ID_OSAL  +  217U)
#define    OSAL_MQ_OSAL_218                           (RESORCE_ID_OSAL  +  218U)
#define    OSAL_MQ_OSAL_219                           (RESORCE_ID_OSAL  +  219U)
#define    OSAL_MQ_OSAL_220                           (RESORCE_ID_OSAL  +  220U)
#define    OSAL_MQ_OSAL_221                           (RESORCE_ID_OSAL  +  221U)
#define    OSAL_MQ_OSAL_222                           (RESORCE_ID_OSAL  +  222U)
#define    OSAL_MQ_OSAL_223                           (RESORCE_ID_OSAL  +  223U)
#define    OSAL_MQ_OSAL_224                           (RESORCE_ID_OSAL  +  224U)
#define    OSAL_MQ_OSAL_225                           (RESORCE_ID_OSAL  +  225U)
#define    OSAL_MQ_OSAL_226                           (RESORCE_ID_OSAL  +  226U)
#define    OSAL_MQ_OSAL_227                           (RESORCE_ID_OSAL  +  227U)
#define    OSAL_MQ_OSAL_228                           (RESORCE_ID_OSAL  +  228U)
#define    OSAL_MQ_OSAL_229                           (RESORCE_ID_OSAL  +  229U)
#define    OSAL_MQ_OSAL_230                           (RESORCE_ID_OSAL  +  230U)
#define    OSAL_MQ_OSAL_231                           (RESORCE_ID_OSAL  +  231U)
#define    OSAL_MQ_OSAL_232                           (RESORCE_ID_OSAL  +  232U)
#define    OSAL_MQ_OSAL_233                           (RESORCE_ID_OSAL  +  233U)
#define    OSAL_MQ_OSAL_234                           (RESORCE_ID_OSAL  +  234U)
#define    OSAL_MQ_OSAL_235                           (RESORCE_ID_OSAL  +  235U)
#define    OSAL_MQ_OSAL_236                           (RESORCE_ID_OSAL  +  236U)
#define    OSAL_MQ_OSAL_237                           (RESORCE_ID_OSAL  +  237U)
#define    OSAL_MQ_OSAL_238                           (RESORCE_ID_OSAL  +  238U)
#define    OSAL_MQ_OSAL_239                           (RESORCE_ID_OSAL  +  239U)
#define    OSAL_MQ_OSAL_240                           (RESORCE_ID_OSAL  +  240U)
#define    OSAL_MQ_OSAL_241                           (RESORCE_ID_OSAL  +  241U)
#define    OSAL_MQ_OSAL_242                           (RESORCE_ID_OSAL  +  242U)
#define    OSAL_MQ_OSAL_243                           (RESORCE_ID_OSAL  +  243U)
#define    OSAL_MQ_OSAL_244                           (RESORCE_ID_OSAL  +  244U)
#define    OSAL_MQ_OSAL_245                           (RESORCE_ID_OSAL  +  245U)
#define    OSAL_MQ_OSAL_246                           (RESORCE_ID_OSAL  +  246U)
#define    OSAL_MQ_OSAL_247                           (RESORCE_ID_OSAL  +  247U)
#define    OSAL_MQ_OSAL_248                           (RESORCE_ID_OSAL  +  248U)
#define    OSAL_MQ_OSAL_249                           (RESORCE_ID_OSAL  +  249U)
#define    OSAL_MQ_OSAL_250                           (RESORCE_ID_OSAL  +  250U)
#define    OSAL_MQ_OSAL_251                           (RESORCE_ID_OSAL  +  251U)
#define    OSAL_MQ_OSAL_252                           (RESORCE_ID_OSAL  +  252U)
#define    OSAL_MQ_OSAL_253                           (RESORCE_ID_OSAL  +  253U)
#define    OSAL_MQ_OSAL_254                           (RESORCE_ID_OSAL  +  254U)
#define    OSAL_MQ_OSAL_255                           (RESORCE_ID_OSAL  +  255U)
#define    OSAL_MQ_OSAL_256                           (RESORCE_ID_OSAL  +  256U)
#define    OSAL_MQ_OSAL_257                           (RESORCE_ID_OSAL  +  257U)
#define    OSAL_MQ_OSAL_258                           (RESORCE_ID_OSAL  +  258U)
#define    OSAL_MQ_OSAL_259                           (RESORCE_ID_OSAL  +  259U)
#define    OSAL_MQ_OSAL_260                           (RESORCE_ID_OSAL  +  260U)
#define    OSAL_MQ_OSAL_261                           (RESORCE_ID_OSAL  +  261U)
#define    OSAL_MQ_OSAL_262                           (RESORCE_ID_OSAL  +  262U)
#define    OSAL_MQ_OSAL_263                           (RESORCE_ID_OSAL  +  263U)
#define    OSAL_MQ_OSAL_264                           (RESORCE_ID_OSAL  +  264U)
#define    OSAL_MQ_OSAL_265                           (RESORCE_ID_OSAL  +  265U)
#define    OSAL_MQ_OSAL_266                           (RESORCE_ID_OSAL  +  266U)
#define    OSAL_MQ_OSAL_267                           (RESORCE_ID_OSAL  +  267U)
#define    OSAL_MQ_OSAL_268                           (RESORCE_ID_OSAL  +  268U)
#define    OSAL_MQ_OSAL_269                           (RESORCE_ID_OSAL  +  269U)
#define    OSAL_MQ_OSAL_270                           (RESORCE_ID_OSAL  +  270U)
#define    OSAL_MQ_OSAL_271                           (RESORCE_ID_OSAL  +  271U)
#define    OSAL_MQ_OSAL_272                           (RESORCE_ID_OSAL  +  272U)
#define    OSAL_MQ_OSAL_273                           (RESORCE_ID_OSAL  +  273U)
#define    OSAL_MQ_OSAL_274                           (RESORCE_ID_OSAL  +  274U)
#define    OSAL_MQ_OSAL_275                           (RESORCE_ID_OSAL  +  275U)
#define    OSAL_MQ_OSAL_276                           (RESORCE_ID_OSAL  +  276U)
#define    OSAL_MQ_OSAL_277                           (RESORCE_ID_OSAL  +  277U)
#define    OSAL_MQ_OSAL_278                           (RESORCE_ID_OSAL  +  278U)
#define    OSAL_MQ_OSAL_279                           (RESORCE_ID_OSAL  +  279U)
#define    OSAL_MQ_OSAL_280                           (RESORCE_ID_OSAL  +  280U)
#define    OSAL_MQ_OSAL_281                           (RESORCE_ID_OSAL  +  281U)
#define    OSAL_MQ_OSAL_282                           (RESORCE_ID_OSAL  +  282U)
#define    OSAL_MQ_OSAL_283                           (RESORCE_ID_OSAL  +  283U)
#define    OSAL_MQ_OSAL_284                           (RESORCE_ID_OSAL  +  284U)
#define    OSAL_MQ_OSAL_285                           (RESORCE_ID_OSAL  +  285U)
#define    OSAL_MQ_OSAL_286                           (RESORCE_ID_OSAL  +  286U)
#define    OSAL_MQ_OSAL_287                           (RESORCE_ID_OSAL  +  287U)
#define    OSAL_MQ_OSAL_288                           (RESORCE_ID_OSAL  +  288U)
#define    OSAL_MQ_OSAL_289                           (RESORCE_ID_OSAL  +  289U)
#define    OSAL_MQ_OSAL_290                           (RESORCE_ID_OSAL  +  290U)
#define    OSAL_MQ_OSAL_291                           (RESORCE_ID_OSAL  +  291U)
#define    OSAL_MQ_OSAL_292                           (RESORCE_ID_OSAL  +  292U)
#define    OSAL_MQ_OSAL_293                           (RESORCE_ID_OSAL  +  293U)
#define    OSAL_MQ_OSAL_294                           (RESORCE_ID_OSAL  +  294U)
#define    OSAL_MQ_OSAL_295                           (RESORCE_ID_OSAL  +  295U)
#define    OSAL_MQ_OSAL_296                           (RESORCE_ID_OSAL  +  296U)
#define    OSAL_MQ_OSAL_297                           (RESORCE_ID_OSAL  +  297U)
#define    OSAL_MQ_OSAL_298                           (RESORCE_ID_OSAL  +  298U)
#define    OSAL_MQ_OSAL_299                           (RESORCE_ID_OSAL  +  299U)
#define    OSAL_MQ_OSAL_300                           (RESORCE_ID_OSAL  +  300U)
#define    OSAL_MQ_OSAL_301                           (RESORCE_ID_OSAL  +  301U)
#define    OSAL_MQ_OSAL_302                           (RESORCE_ID_OSAL  +  302U)
#define    OSAL_MQ_OSAL_303                           (RESORCE_ID_OSAL  +  303U)
#define    OSAL_MQ_OSAL_304                           (RESORCE_ID_OSAL  +  304U)
#define    OSAL_MQ_OSAL_305                           (RESORCE_ID_OSAL  +  305U)
#define    OSAL_MQ_OSAL_306                           (RESORCE_ID_OSAL  +  306U)
#define    OSAL_MQ_OSAL_307                           (RESORCE_ID_OSAL  +  307U)
#define    OSAL_MQ_OSAL_308                           (RESORCE_ID_OSAL  +  308U)
#define    OSAL_MQ_OSAL_309                           (RESORCE_ID_OSAL  +  309U)
#define    OSAL_MQ_OSAL_310                           (RESORCE_ID_OSAL  +  310U)
#define    OSAL_MQ_OSAL_311                           (RESORCE_ID_OSAL  +  311U)
#define    OSAL_MQ_OSAL_312                           (RESORCE_ID_OSAL  +  312U)
#define    OSAL_MQ_OSAL_313                           (RESORCE_ID_OSAL  +  313U)
#define    OSAL_MQ_OSAL_314                           (RESORCE_ID_OSAL  +  314U)
#define    OSAL_MQ_OSAL_315                           (RESORCE_ID_OSAL  +  315U)
#define    OSAL_MQ_OSAL_316                           (RESORCE_ID_OSAL  +  316U)
#define    OSAL_MQ_OSAL_317                           (RESORCE_ID_OSAL  +  317U)
#define    OSAL_MQ_OSAL_318                           (RESORCE_ID_OSAL  +  318U)
#define    OSAL_MQ_OSAL_319                           (RESORCE_ID_OSAL  +  319U)
#define    OSAL_MQ_OSAL_320                           (RESORCE_ID_OSAL  +  320U)
#define    OSAL_MQ_OSAL_321                           (RESORCE_ID_OSAL  +  321U)
#define    OSAL_MQ_OSAL_322                           (RESORCE_ID_OSAL  +  322U)
#define    OSAL_MQ_OSAL_323                           (RESORCE_ID_OSAL  +  323U)
#define    OSAL_MQ_OSAL_324                           (RESORCE_ID_OSAL  +  324U)
#define    OSAL_MQ_OSAL_325                           (RESORCE_ID_OSAL  +  325U)
#define    OSAL_MQ_OSAL_326                           (RESORCE_ID_OSAL  +  326U)
#define    OSAL_MQ_OSAL_327                           (RESORCE_ID_OSAL  +  327U)
#define    OSAL_MQ_OSAL_328                           (RESORCE_ID_OSAL  +  328U)
#define    OSAL_MQ_OSAL_329                           (RESORCE_ID_OSAL  +  329U)
#define    OSAL_MQ_OSAL_330                           (RESORCE_ID_OSAL  +  330U)
#define    OSAL_MQ_OSAL_331                           (RESORCE_ID_OSAL  +  331U)
#define    OSAL_MQ_OSAL_332                           (RESORCE_ID_OSAL  +  332U)
#define    OSAL_MQ_OSAL_333                           (RESORCE_ID_OSAL  +  333U)
#define    OSAL_MQ_OSAL_334                           (RESORCE_ID_OSAL  +  334U)
#define    OSAL_MQ_OSAL_335                           (RESORCE_ID_OSAL  +  335U)
#define    OSAL_MQ_OSAL_336                           (RESORCE_ID_OSAL  +  336U)
#define    OSAL_MQ_OSAL_337                           (RESORCE_ID_OSAL  +  337U)
#define    OSAL_MQ_OSAL_338                           (RESORCE_ID_OSAL  +  338U)
#define    OSAL_MQ_OSAL_339                           (RESORCE_ID_OSAL  +  339U)
#define    OSAL_MQ_OSAL_340                           (RESORCE_ID_OSAL  +  340U)
#define    OSAL_MQ_OSAL_341                           (RESORCE_ID_OSAL  +  341U)
#define    OSAL_MQ_OSAL_342                           (RESORCE_ID_OSAL  +  342U)
#define    OSAL_MQ_OSAL_343                           (RESORCE_ID_OSAL  +  343U)
#define    OSAL_MQ_OSAL_344                           (RESORCE_ID_OSAL  +  344U)
#define    OSAL_MQ_OSAL_345                           (RESORCE_ID_OSAL  +  345U)
#define    OSAL_MQ_OSAL_346                           (RESORCE_ID_OSAL  +  346U)
#define    OSAL_MQ_OSAL_347                           (RESORCE_ID_OSAL  +  347U)
#define    OSAL_MQ_OSAL_348                           (RESORCE_ID_OSAL  +  348U)
#define    OSAL_MQ_OSAL_349                           (RESORCE_ID_OSAL  +  349U)
#define    OSAL_MQ_OSAL_350                           (RESORCE_ID_OSAL  +  350U)
#define    OSAL_MQ_OSAL_351                           (RESORCE_ID_OSAL  +  351U)
#define    OSAL_MQ_OSAL_352                           (RESORCE_ID_OSAL  +  352U)
#define    OSAL_MQ_OSAL_353                           (RESORCE_ID_OSAL  +  353U)
#define    OSAL_MQ_OSAL_354                           (RESORCE_ID_OSAL  +  354U)
#define    OSAL_MQ_OSAL_355                           (RESORCE_ID_OSAL  +  355U)
#define    OSAL_MQ_OSAL_356                           (RESORCE_ID_OSAL  +  356U)
#define    OSAL_MQ_OSAL_357                           (RESORCE_ID_OSAL  +  357U)
#define    OSAL_MQ_OSAL_358                           (RESORCE_ID_OSAL  +  358U)
#define    OSAL_MQ_OSAL_359                           (RESORCE_ID_OSAL  +  359U)
#define    OSAL_MQ_OSAL_360                           (RESORCE_ID_OSAL  +  360U)
#define    OSAL_MQ_OSAL_361                           (RESORCE_ID_OSAL  +  361U)
#define    OSAL_MQ_OSAL_362                           (RESORCE_ID_OSAL  +  362U)
#define    OSAL_MQ_OSAL_363                           (RESORCE_ID_OSAL  +  363U)
#define    OSAL_MQ_OSAL_364                           (RESORCE_ID_OSAL  +  364U)
#define    OSAL_MQ_OSAL_365                           (RESORCE_ID_OSAL  +  365U)
#define    OSAL_MQ_OSAL_366                           (RESORCE_ID_OSAL  +  366U)
#define    OSAL_MQ_OSAL_367                           (RESORCE_ID_OSAL  +  367U)
#define    OSAL_MQ_OSAL_368                           (RESORCE_ID_OSAL  +  368U)
#define    OSAL_MQ_OSAL_369                           (RESORCE_ID_OSAL  +  369U)
#define    OSAL_MQ_OSAL_370                           (RESORCE_ID_OSAL  +  370U)
#define    OSAL_MQ_OSAL_371                           (RESORCE_ID_OSAL  +  371U)
#define    OSAL_MQ_OSAL_372                           (RESORCE_ID_OSAL  +  372U)
#define    OSAL_MQ_OSAL_373                           (RESORCE_ID_OSAL  +  373U)
#define    OSAL_MQ_OSAL_374                           (RESORCE_ID_OSAL  +  374U)
#define    OSAL_MQ_OSAL_375                           (RESORCE_ID_OSAL  +  375U)
#define    OSAL_MQ_OSAL_376                           (RESORCE_ID_OSAL  +  376U)
#define    OSAL_MQ_OSAL_377                           (RESORCE_ID_OSAL  +  377U)
#define    OSAL_MQ_OSAL_378                           (RESORCE_ID_OSAL  +  378U)
#define    OSAL_MQ_OSAL_379                           (RESORCE_ID_OSAL  +  379U)
#define    OSAL_MQ_OSAL_380                           (RESORCE_ID_OSAL  +  380U)
#define    OSAL_MQ_OSAL_381                           (RESORCE_ID_OSAL  +  381U)
#define    OSAL_MQ_OSAL_382                           (RESORCE_ID_OSAL  +  382U)
#define    OSAL_MQ_OSAL_383                           (RESORCE_ID_OSAL  +  383U)
#define    OSAL_MQ_OSAL_384                           (RESORCE_ID_OSAL  +  384U)
#define    OSAL_MQ_OSAL_385                           (RESORCE_ID_OSAL  +  385U)
#define    OSAL_MQ_OSAL_386                           (RESORCE_ID_OSAL  +  386U)
#define    OSAL_MQ_OSAL_387                           (RESORCE_ID_OSAL  +  387U)
#define    OSAL_MQ_OSAL_388                           (RESORCE_ID_OSAL  +  388U)
#define    OSAL_MQ_OSAL_389                           (RESORCE_ID_OSAL  +  389U)
#define    OSAL_MQ_OSAL_390                           (RESORCE_ID_OSAL  +  390U)
#define    OSAL_MQ_OSAL_391                           (RESORCE_ID_OSAL  +  391U)
#define    OSAL_MQ_OSAL_392                           (RESORCE_ID_OSAL  +  392U)
#define    OSAL_MQ_OSAL_393                           (RESORCE_ID_OSAL  +  393U)
#define    OSAL_MQ_OSAL_394                           (RESORCE_ID_OSAL  +  394U)
#define    OSAL_MQ_OSAL_395                           (RESORCE_ID_OSAL  +  395U)
#define    OSAL_MQ_OSAL_396                           (RESORCE_ID_OSAL  +  396U)
#define    OSAL_MQ_OSAL_397                           (RESORCE_ID_OSAL  +  397U)
#define    OSAL_MQ_OSAL_398                           (RESORCE_ID_OSAL  +  398U)
#define    OSAL_MQ_OSAL_399                           (RESORCE_ID_OSAL  +  399U)
#define    OSAL_MQ_OSAL_400                           (RESORCE_ID_OSAL  +  400U)
#define    OSAL_MQ_OSAL_401                           (RESORCE_ID_OSAL  +  401U)
#define    OSAL_MQ_OSAL_402                           (RESORCE_ID_OSAL  +  402U)
#define    OSAL_MQ_OSAL_403                           (RESORCE_ID_OSAL  +  403U)
#define    OSAL_MQ_OSAL_404                           (RESORCE_ID_OSAL  +  404U)
#define    OSAL_MQ_OSAL_405                           (RESORCE_ID_OSAL  +  405U)
#define    OSAL_MQ_OSAL_406                           (RESORCE_ID_OSAL  +  406U)
#define    OSAL_MQ_OSAL_407                           (RESORCE_ID_OSAL  +  407U)
#define    OSAL_MQ_OSAL_408                           (RESORCE_ID_OSAL  +  408U)
#define    OSAL_MQ_OSAL_409                           (RESORCE_ID_OSAL  +  409U)
#define    OSAL_MQ_OSAL_410                           (RESORCE_ID_OSAL  +  410U)
#define    OSAL_MQ_OSAL_411                           (RESORCE_ID_OSAL  +  411U)
#define    OSAL_MQ_OSAL_412                           (RESORCE_ID_OSAL  +  412U)
#define    OSAL_MQ_OSAL_413                           (RESORCE_ID_OSAL  +  413U)
#define    OSAL_MQ_OSAL_414                           (RESORCE_ID_OSAL  +  414U)
#define    OSAL_MQ_OSAL_415                           (RESORCE_ID_OSAL  +  415U)
#define    OSAL_MQ_OSAL_416                           (RESORCE_ID_OSAL  +  416U)
#define    OSAL_MQ_OSAL_417                           (RESORCE_ID_OSAL  +  417U)
#define    OSAL_MQ_OSAL_418                           (RESORCE_ID_OSAL  +  418U)
#define    OSAL_MQ_OSAL_419                           (RESORCE_ID_OSAL  +  419U)
#define    OSAL_MQ_OSAL_420                           (RESORCE_ID_OSAL  +  420U)
#define    OSAL_MQ_OSAL_421                           (RESORCE_ID_OSAL  +  421U)
#define    OSAL_MQ_OSAL_422                           (RESORCE_ID_OSAL  +  422U)
#define    OSAL_MQ_OSAL_423                           (RESORCE_ID_OSAL  +  423U)
#define    OSAL_MQ_OSAL_424                           (RESORCE_ID_OSAL  +  424U)
#define    OSAL_MQ_OSAL_425                           (RESORCE_ID_OSAL  +  425U)
#define    OSAL_MQ_OSAL_426                           (RESORCE_ID_OSAL  +  426U)
#define    OSAL_MQ_OSAL_427                           (RESORCE_ID_OSAL  +  427U)
#define    OSAL_MQ_OSAL_428                           (RESORCE_ID_OSAL  +  428U)
#define    OSAL_MQ_OSAL_429                           (RESORCE_ID_OSAL  +  429U)
#define    OSAL_MQ_OSAL_430                           (RESORCE_ID_OSAL  +  430U)
#define    OSAL_MQ_OSAL_431                           (RESORCE_ID_OSAL  +  431U)
#define    OSAL_MQ_OSAL_432                           (RESORCE_ID_OSAL  +  432U)
#define    OSAL_MQ_OSAL_433                           (RESORCE_ID_OSAL  +  433U)
#define    OSAL_MQ_OSAL_434                           (RESORCE_ID_OSAL  +  434U)
#define    OSAL_MQ_OSAL_435                           (RESORCE_ID_OSAL  +  435U)
#define    OSAL_MQ_OSAL_436                           (RESORCE_ID_OSAL  +  436U)
#define    OSAL_MQ_OSAL_437                           (RESORCE_ID_OSAL  +  437U)
#define    OSAL_MQ_OSAL_438                           (RESORCE_ID_OSAL  +  438U)
#define    OSAL_MQ_OSAL_439                           (RESORCE_ID_OSAL  +  439U)
#define    OSAL_MQ_OSAL_440                           (RESORCE_ID_OSAL  +  440U)
#define    OSAL_MQ_OSAL_441                           (RESORCE_ID_OSAL  +  441U)
#define    OSAL_MQ_OSAL_442                           (RESORCE_ID_OSAL  +  442U)
#define    OSAL_MQ_OSAL_443                           (RESORCE_ID_OSAL  +  443U)
#define    OSAL_MQ_OSAL_444                           (RESORCE_ID_OSAL  +  444U)
#define    OSAL_MQ_OSAL_445                           (RESORCE_ID_OSAL  +  445U)
#define    OSAL_MQ_OSAL_446                           (RESORCE_ID_OSAL  +  446U)
#define    OSAL_MQ_OSAL_447                           (RESORCE_ID_OSAL  +  447U)
#define    OSAL_MQ_OSAL_448                           (RESORCE_ID_OSAL  +  448U)
#define    OSAL_MQ_OSAL_449                           (RESORCE_ID_OSAL  +  449U)
#define    OSAL_MQ_OSAL_450                           (RESORCE_ID_OSAL  +  450U)
#define    OSAL_MQ_OSAL_451                           (RESORCE_ID_OSAL  +  451U)
#define    OSAL_MQ_OSAL_452                           (RESORCE_ID_OSAL  +  452U)
#define    OSAL_MQ_OSAL_453                           (RESORCE_ID_OSAL  +  453U)
#define    OSAL_MQ_OSAL_454                           (RESORCE_ID_OSAL  +  454U)
#define    OSAL_MQ_OSAL_455                           (RESORCE_ID_OSAL  +  455U)
#define    OSAL_MQ_OSAL_456                           (RESORCE_ID_OSAL  +  456U)
#define    OSAL_MQ_OSAL_457                           (RESORCE_ID_OSAL  +  457U)
#define    OSAL_MQ_OSAL_458                           (RESORCE_ID_OSAL  +  458U)
#define    OSAL_MQ_OSAL_459                           (RESORCE_ID_OSAL  +  459U)
#define    OSAL_MQ_OSAL_460                           (RESORCE_ID_OSAL  +  460U)
#define    OSAL_MQ_OSAL_461                           (RESORCE_ID_OSAL  +  461U)
#define    OSAL_MQ_OSAL_462                           (RESORCE_ID_OSAL  +  462U)
#define    OSAL_MQ_OSAL_463                           (RESORCE_ID_OSAL  +  463U)
#define    OSAL_MQ_OSAL_464                           (RESORCE_ID_OSAL  +  464U)
#define    OSAL_MQ_OSAL_465                           (RESORCE_ID_OSAL  +  465U)
#define    OSAL_MQ_OSAL_466                           (RESORCE_ID_OSAL  +  466U)
#define    OSAL_MQ_OSAL_467                           (RESORCE_ID_OSAL  +  467U)
#define    OSAL_MQ_OSAL_468                           (RESORCE_ID_OSAL  +  468U)
#define    OSAL_MQ_OSAL_469                           (RESORCE_ID_OSAL  +  469U)
#define    OSAL_MQ_OSAL_470                           (RESORCE_ID_OSAL  +  470U)
#define    OSAL_MQ_OSAL_471                           (RESORCE_ID_OSAL  +  471U)
#define    OSAL_MQ_OSAL_472                           (RESORCE_ID_OSAL  +  472U)
#define    OSAL_MQ_OSAL_473                           (RESORCE_ID_OSAL  +  473U)
#define    OSAL_MQ_OSAL_474                           (RESORCE_ID_OSAL  +  474U)
#define    OSAL_MQ_OSAL_475                           (RESORCE_ID_OSAL  +  475U)
#define    OSAL_MQ_OSAL_476                           (RESORCE_ID_OSAL  +  476U)
#define    OSAL_MQ_OSAL_477                           (RESORCE_ID_OSAL  +  477U)
#define    OSAL_MQ_OSAL_478                           (RESORCE_ID_OSAL  +  478U)
#define    OSAL_MQ_OSAL_479                           (RESORCE_ID_OSAL  +  479U)
#define    OSAL_MQ_OSAL_480                           (RESORCE_ID_OSAL  +  480U)
#define    OSAL_MQ_OSAL_481                           (RESORCE_ID_OSAL  +  481U)
#define    OSAL_MQ_OSAL_482                           (RESORCE_ID_OSAL  +  482U)
#define    OSAL_MQ_OSAL_483                           (RESORCE_ID_OSAL  +  483U)
#define    OSAL_MQ_OSAL_484                           (RESORCE_ID_OSAL  +  484U)
#define    OSAL_MQ_OSAL_485                           (RESORCE_ID_OSAL  +  485U)
#define    OSAL_MQ_OSAL_486                           (RESORCE_ID_OSAL  +  486U)
#define    OSAL_MQ_OSAL_487                           (RESORCE_ID_OSAL  +  487U)
#define    OSAL_MQ_OSAL_488                           (RESORCE_ID_OSAL  +  488U)
#define    OSAL_MQ_OSAL_489                           (RESORCE_ID_OSAL  +  489U)
#define    OSAL_MQ_OSAL_490                           (RESORCE_ID_OSAL  +  490U)
#define    OSAL_MQ_OSAL_491                           (RESORCE_ID_OSAL  +  491U)
#define    OSAL_MQ_OSAL_492                           (RESORCE_ID_OSAL  +  492U)
#define    OSAL_MQ_OSAL_493                           (RESORCE_ID_OSAL  +  493U)
#define    OSAL_MQ_OSAL_494                           (RESORCE_ID_OSAL  +  494U)
#define    OSAL_MQ_OSAL_495                           (RESORCE_ID_OSAL  +  495U)
#define    OSAL_MQ_OSAL_496                           (RESORCE_ID_OSAL  +  496U)
#define    OSAL_MQ_OSAL_497                           (RESORCE_ID_OSAL  +  497U)
#define    OSAL_MQ_OSAL_498                           (RESORCE_ID_OSAL  +  498U)
#define    OSAL_MQ_OSAL_499                           (RESORCE_ID_OSAL  +  499U)
#define    OSAL_MQ_OSAL_500                           (RESORCE_ID_OSAL  +  500U)

/*******************************************************************************************************************//**
 * @var g_osal_device_soc_type
 * osal device configuration : SoC type
***********************************************************************************************************************/
#ifndef TEST_LINUX
const st_osal_device_soc_type_t g_osal_device_soc_type =
{
#if defined(RCAR_V3M)
    .device_soc_type = (uint64_t)OSAL_DEVICE_SOC_TYPE_V3M
#elif defined(RCAR_V3H1)
    .device_soc_type = (uint64_t)OSAL_DEVICE_SOC_TYPE_V3H1
#elif defined(RCAR_V3H2)
    .device_soc_type = (uint64_t)OSAL_DEVICE_SOC_TYPE_V3H2
#elif defined(RCAR_V4H2)
    .device_soc_type = (uint64_t)OSAL_DEVICE_SOC_TYPE_V4H2
#elif defined(RCAR_V4H)
    .device_soc_type = (uint64_t)OSAL_DEVICE_SOC_TYPE_V4H
#elif defined(RCAR_V4M)
    .device_soc_type = (uint64_t)OSAL_DEVICE_SOC_TYPE_V4M
#elif defined(RCAR_S4) || defined(RCAR_S4N)
    .device_soc_type = (uint64_t)OSAL_DEVICE_SOC_TYPE_S4
#else
    .device_soc_type = (uint64_t)OSAL_DEVICE_SOC_TYPE_INVALID
#endif
};
#endif

/*******************************************************************************************************************//**
 * @var g_osal_memory_max_configration
 * osal device configuration : max memory configuration structure
***********************************************************************************************************************/
const st_osal_mmngr_config_t g_osal_memory_max_configration =
{
    .mode = OSAL_MMNGR_ALLOC_MODE_STACK_PROT,
    .memory_limit = (512U * 1024U * 1024U),
    .max_allowed_allocations = (4U * 1024U),
    .max_registered_monitors_cbs = (4U * 1024U)
};

/*******************************************************************************************************************//**
 * @var g_osal_memory_region_info
 * osal device configuration : memory region information structure
***********************************************************************************************************************/
const st_osal_mmngr_region_info_list_t g_osal_memory_region_info_list =
{
#if defined (TEST_LINUX)
    .num_of_memory_regions = 2,
    .default_memory_region = 0,
    .p_info = (st_osal_mmngr_region_info_t[])
    {
        /* Memory region 0 */
        {
            .name = "/dev/cmem0",
            .offset = 0,
            .range = (512U * 1024U * 1024U)
        },
        /* Memory region 1 */ /* example */
        {
            .name = "/dev/cmem1",
            .offset = 0,
            .range = (16U * 1024U * 1024U)
        }
    }
#endif

#if defined (TEST_QNX)
    .num_of_memory_regions = 2,
    .default_memory_region = 0,
    .p_info = (st_osal_mmngr_region_info_t[])
    {
#if defined(RCAR_V4H) || defined(RCAR_V4M)
        /* Memory region 0(default) */
        {
            .name = "",
            .offset = 0x80000000,
            .range = (512U * 1024U * 1024U)
        },
#else
        /* Memory region 0(default) */
        {
            .name = "sysram&below4G",      /* The default region requires "below 4G area" */
            .offset = 0,
            .range = (512U * 1024U * 1024U)
        },
#endif
        /* Memory region 1 */ /* example */
        {
            .name = "",
            .offset = 0,
            .range = (64U * 1024U * 1024U)
        }
    }
#endif

#if defined (TEST_EMCOS)
    .num_of_memory_regions = 2,
    .default_memory_region = 0,
    .p_info = (st_osal_mmngr_region_info_t[])
    {
        {
            .name = "/pdev/mem",
            .offset = 0,
            .range = (384U * 1024U * 1024U)
        },
        {
            .name = "/pdev/mem",
            .offset =(384U * 1024U * 1024U)
                     + (((4096U * 2U) + 4096U) * 1024U), /* region 0 end + OSAL Memory Manager reserve */
            .range = (128U * 1024U * 1024U)
        }
    }
#endif
};

#ifdef TEST_LINUX
/***********************************************************************************************************************
 * @var g_osal_thread_affinity
 * @brief osal resource configuration : CPU affinity value for thread manager.
 **********************************************************************************************************************/
const st_osal_thread_affinity_t g_osal_thread_affinity[] =
{
    {.id = OSAL_THREAD_AFFINITY_INVALID_ID}
};

/***********************************************************************************************************************
 * @var g_osal_interrupt_thread_affinity
 * @brief osal device configuration : CPU affinity value for interrupt manager.
***********************************************************************************************************************/
const st_osal_interrupt_thread_affinity_t  g_osal_interrupt_thread_affinity[] =
{
    {.spi_num = OSAL_INTERRUPT_AFFINITY_INVALID_ID}
};
#endif

#ifdef TEST_QNX
/*******************************************************************************************************************//**
 * @var g_osal_thread_priority_matrix
 * @brief osal resource configuration : Matrix of thread priorities
***********************************************************************************************************************/
const int32_t g_osal_thread_priority_matrix[15] =
{
        1,          /* OSAL_THREAD_PRIORITY_TYPE0  */
        2,          /* OSAL_THREAD_PRIORITY_TYPE1  */
        3,          /* OSAL_THREAD_PRIORITY_TYPE2  */
        4,          /* OSAL_THREAD_PRIORITY_TYPE3  */
        5,          /* OSAL_THREAD_PRIORITY_TYPE4  */
        6,          /* OSAL_THREAD_PRIORITY_TYPE5  */
        7,          /* OSAL_THREAD_PRIORITY_TYPE6  */
        8,          /* OSAL_THREAD_PRIORITY_TYPE7  */
        9,          /* OSAL_THREAD_PRIORITY_TYPE8  */
        10,         /* OSAL_THREAD_PRIORITY_TYPE9  */
        11,         /* OSAL_THREAD_PRIORITY_TYPE10 */
        12,         /* OSAL_THREAD_PRIORITY_TYPE11 */
        13,         /* OSAL_THREAD_PRIORITY_TYPE12 */
        14,         /* OSAL_THREAD_PRIORITY_TYPE13 */
        15          /* OSAL_THREAD_PRIORITY_TYPE14 */
};

/*******************************************************************************************************************//**
 * @var g_osal_interrupt_thread_priority_matrix
 * @brief osal device configuration : Matrix of interrupt thread priorities
***********************************************************************************************************************/
const int32_t g_osal_interrupt_thread_priority_matrix[15] =
{
        16,         /* OSAL_INTERRUPT_PRIORITY_TYPE0  */
        17,         /* OSAL_INTERRUPT_PRIORITY_TYPE1  */
        18,         /* OSAL_INTERRUPT_PRIORITY_TYPE2  */
        19,         /* OSAL_INTERRUPT_PRIORITY_TYPE3  */
        20,         /* OSAL_INTERRUPT_PRIORITY_TYPE4  */
        21,         /* OSAL_INTERRUPT_PRIORITY_TYPE5  */
        22,         /* OSAL_INTERRUPT_PRIORITY_TYPE6  */
        23,         /* OSAL_INTERRUPT_PRIORITY_TYPE7  */
        24,         /* OSAL_INTERRUPT_PRIORITY_TYPE8  */
        25,         /* OSAL_INTERRUPT_PRIORITY_TYPE9  */
        26,         /* OSAL_INTERRUPT_PRIORITY_TYPE10 */
        27,         /* OSAL_INTERRUPT_PRIORITY_TYPE11 */
        28,         /* OSAL_INTERRUPT_PRIORITY_TYPE12 */
        29,         /* OSAL_INTERRUPT_PRIORITY_TYPE13 */
        30          /* OSAL_INTERRUPT_PRIORITY_TYPE14 */
};

/*******************************************************************************************************************//**
 * @var g_osal_internal_thread_affinity
 * @brief osal resource configuration : CPU Affinity value for threads managed independently by OSAL
***********************************************************************************************************************/
const uint64_t g_osal_internal_thread_affinity = 0x1U;

/* Main thread CPU Affinity for Test */
uint64_t g_osal_main_thread_cpu_affinity = 0x1U;

/*******************************************************************************************************************//**
 * @var g_osal_thread_affinity
 * @brief osal resource configuration : CPU affinity value for thread
***********************************************************************************************************************/
const st_osal_thread_affinity_t g_osal_thread_affinity[] =
{
    /* Set CPU core0 */
    {OSAL_THREAD_OSAL_00,           0x1U },
    {OSAL_THREAD_OSAL_01,           0x1U },
    {OSAL_THREAD_OSAL_02,           0x1U },
    {OSAL_THREAD_OSAL_03,           0x1U },
    {OSAL_THREAD_OSAL_04,           0x1U },
    {OSAL_THREAD_OSAL_05,           0x1U },
    {OSAL_THREAD_OSAL_06,           0x1U },
    {OSAL_THREAD_OSAL_07,           0x1U },
    {OSAL_THREAD_OSAL_08,           0x1U },
    {OSAL_THREAD_OSAL_09,           0x1U },
    {OSAL_THREAD_OSAL_10,           0x1U },
#if defined(ENABLE_LONGTIME_TEST)
    {OSAL_THREAD_OSAL_11,           0x1U },
    {OSAL_THREAD_OSAL_12,           0x1U },
    {OSAL_THREAD_OSAL_13,           0x1U },
    {OSAL_THREAD_OSAL_14,           0x1U },
    {OSAL_THREAD_OSAL_15,           0x1U },
    {OSAL_THREAD_OSAL_16,           0x1U },
    {OSAL_THREAD_OSAL_17,           0x1U },
    {OSAL_THREAD_OSAL_18,           0x1U },
    {OSAL_THREAD_OSAL_19,           0x1U },
    {OSAL_THREAD_OSAL_20,           0x1U },
    {OSAL_THREAD_OSAL_21,           0x1U },
    {OSAL_THREAD_OSAL_22,           0x1U },
    {OSAL_THREAD_OSAL_23,           0x1U },
    {OSAL_THREAD_OSAL_24,           0x1U },
    {OSAL_THREAD_OSAL_25,           0x1U },
    {OSAL_THREAD_OSAL_26,           0x1U },
    {OSAL_THREAD_OSAL_27,           0x1U },
    {OSAL_THREAD_OSAL_28,           0x1U },
    {OSAL_THREAD_OSAL_29,           0x1U },
    {OSAL_THREAD_OSAL_30,           0x1U },
    {OSAL_THREAD_OSAL_31,           0x1U },
    {OSAL_THREAD_OSAL_32,           0x1U },
    {OSAL_THREAD_OSAL_33,           0x1U },
    {OSAL_THREAD_OSAL_34,           0x1U },
    {OSAL_THREAD_OSAL_35,           0x1U },
    {OSAL_THREAD_OSAL_36,           0x1U },
    {OSAL_THREAD_OSAL_37,           0x1U },
    {OSAL_THREAD_OSAL_38,           0x1U },
    {OSAL_THREAD_OSAL_39,           0x1U },
    {OSAL_THREAD_OSAL_40,           0x1U },
    {OSAL_THREAD_OSAL_41,           0x1U },
    {OSAL_THREAD_OSAL_42,           0x1U },
    {OSAL_THREAD_OSAL_43,           0x1U },
    {OSAL_THREAD_OSAL_44,           0x1U },
    {OSAL_THREAD_OSAL_45,           0x1U },
    {OSAL_THREAD_OSAL_46,           0x1U },
    {OSAL_THREAD_OSAL_47,           0x1U },
    {OSAL_THREAD_OSAL_48,           0x1U },
    {OSAL_THREAD_OSAL_49,           0x1U },
    {OSAL_THREAD_OSAL_50,           0x1U },
    {OSAL_THREAD_OSAL_51,           0x1U },
    {OSAL_THREAD_OSAL_52,           0x1U },
    {OSAL_THREAD_OSAL_53,           0x1U },
    {OSAL_THREAD_OSAL_54,           0x1U },
    {OSAL_THREAD_OSAL_55,           0x1U },
    {OSAL_THREAD_OSAL_56,           0x1U },
    {OSAL_THREAD_OSAL_57,           0x1U },
    {OSAL_THREAD_OSAL_58,           0x1U },
    {OSAL_THREAD_OSAL_59,           0x1U },
    {OSAL_THREAD_OSAL_60,           0x1U },
    {OSAL_THREAD_OSAL_61,           0x1U },
    {OSAL_THREAD_OSAL_62,           0x1U },
    {OSAL_THREAD_OSAL_63,           0x1U },
    {OSAL_THREAD_OSAL_64,           0x1U },
    {OSAL_THREAD_OSAL_65,           0x1U },
    {OSAL_THREAD_OSAL_66,           0x1U },
    {OSAL_THREAD_OSAL_67,           0x1U },
    {OSAL_THREAD_OSAL_68,           0x1U },
    {OSAL_THREAD_OSAL_69,           0x1U },
    {OSAL_THREAD_OSAL_70,           0x1U },
    {OSAL_THREAD_OSAL_71,           0x1U },
    {OSAL_THREAD_OSAL_72,           0x1U },
    {OSAL_THREAD_OSAL_73,           0x1U },
    {OSAL_THREAD_OSAL_74,           0x1U },
    {OSAL_THREAD_OSAL_75,           0x1U },
    {OSAL_THREAD_OSAL_76,           0x1U },
    {OSAL_THREAD_OSAL_77,           0x1U },
    {OSAL_THREAD_OSAL_78,           0x1U },
    {OSAL_THREAD_OSAL_79,           0x1U },
    {OSAL_THREAD_OSAL_80,           0x1U },
    {OSAL_THREAD_OSAL_81,           0x1U },
    {OSAL_THREAD_OSAL_82,           0x1U },
    {OSAL_THREAD_OSAL_83,           0x1U },
    {OSAL_THREAD_OSAL_84,           0x1U },
    {OSAL_THREAD_OSAL_85,           0x1U },
    {OSAL_THREAD_OSAL_86,           0x1U },
    {OSAL_THREAD_OSAL_87,           0x1U },
    {OSAL_THREAD_OSAL_88,           0x1U },
    {OSAL_THREAD_OSAL_89,           0x1U },
    {OSAL_THREAD_OSAL_90,           0x1U },
    {OSAL_THREAD_OSAL_91,           0x1U },
    {OSAL_THREAD_OSAL_92,           0x1U },
    {OSAL_THREAD_OSAL_93,           0x1U },
    {OSAL_THREAD_OSAL_94,           0x1U },
    {OSAL_THREAD_OSAL_95,           0x1U },
    {OSAL_THREAD_OSAL_96,           0x1U },
    {OSAL_THREAD_OSAL_97,           0x1U },
    {OSAL_THREAD_OSAL_98,           0x1U },
    {OSAL_THREAD_OSAL_99,           0x1U },
    {OSAL_THREAD_OSAL_100,          0x1U },
    {OSAL_THREAD_OSAL_101,          0x1U },
    {OSAL_THREAD_OSAL_102,          0x1U },
    {OSAL_THREAD_OSAL_103,          0x1U },
    {OSAL_THREAD_OSAL_104,          0x1U },
    {OSAL_THREAD_OSAL_105,          0x1U },
    {OSAL_THREAD_OSAL_106,          0x1U },
    {OSAL_THREAD_OSAL_107,          0x1U },
    {OSAL_THREAD_OSAL_108,          0x1U },
    {OSAL_THREAD_OSAL_109,          0x1U },
    {OSAL_THREAD_OSAL_110,          0x1U },
    {OSAL_THREAD_OSAL_111,          0x1U },
    {OSAL_THREAD_OSAL_112,          0x1U },
    {OSAL_THREAD_OSAL_113,          0x1U },
    {OSAL_THREAD_OSAL_114,          0x1U },
    {OSAL_THREAD_OSAL_115,          0x1U },
    {OSAL_THREAD_OSAL_116,          0x1U },
    {OSAL_THREAD_OSAL_117,          0x1U },
    {OSAL_THREAD_OSAL_118,          0x1U },
    {OSAL_THREAD_OSAL_119,          0x1U },
    {OSAL_THREAD_OSAL_120,          0x1U },
    {OSAL_THREAD_OSAL_121,          0x1U },
    {OSAL_THREAD_OSAL_122,          0x1U },
    {OSAL_THREAD_OSAL_123,          0x1U },
    {OSAL_THREAD_OSAL_124,          0x1U },
    {OSAL_THREAD_OSAL_125,          0x1U },
    {OSAL_THREAD_OSAL_126,          0x1U },
    {OSAL_THREAD_OSAL_127,          0x1U },
    {OSAL_THREAD_OSAL_128,          0x1U },
    {OSAL_THREAD_OSAL_129,          0x1U },
    {OSAL_THREAD_OSAL_130,          0x1U },
    {OSAL_THREAD_OSAL_131,          0x1U },
    {OSAL_THREAD_OSAL_132,          0x1U },
    {OSAL_THREAD_OSAL_133,          0x1U },
    {OSAL_THREAD_OSAL_134,          0x1U },
    {OSAL_THREAD_OSAL_135,          0x1U },
    {OSAL_THREAD_OSAL_136,          0x1U },
    {OSAL_THREAD_OSAL_137,          0x1U },
    {OSAL_THREAD_OSAL_138,          0x1U },
    {OSAL_THREAD_OSAL_139,          0x1U },
    {OSAL_THREAD_OSAL_140,          0x1U },
    {OSAL_THREAD_OSAL_141,          0x1U },
    {OSAL_THREAD_OSAL_142,          0x1U },
    {OSAL_THREAD_OSAL_143,          0x1U },
    {OSAL_THREAD_OSAL_144,          0x1U },
    {OSAL_THREAD_OSAL_145,          0x1U },
    {OSAL_THREAD_OSAL_146,          0x1U },
    {OSAL_THREAD_OSAL_147,          0x1U },
    {OSAL_THREAD_OSAL_148,          0x1U },
    {OSAL_THREAD_OSAL_149,          0x1U },
    {OSAL_THREAD_OSAL_150,          0x1U },
    {OSAL_THREAD_OSAL_151,          0x1U },
    {OSAL_THREAD_OSAL_152,          0x1U },
    {OSAL_THREAD_OSAL_153,          0x1U },
    {OSAL_THREAD_OSAL_154,          0x1U },
    {OSAL_THREAD_OSAL_155,          0x1U },
    {OSAL_THREAD_OSAL_156,          0x1U },
    {OSAL_THREAD_OSAL_157,          0x1U },
    {OSAL_THREAD_OSAL_158,          0x1U },
    {OSAL_THREAD_OSAL_159,          0x1U },
    {OSAL_THREAD_OSAL_160,          0x1U },
    {OSAL_THREAD_OSAL_161,          0x1U },
    {OSAL_THREAD_OSAL_162,          0x1U },
    {OSAL_THREAD_OSAL_163,          0x1U },
    {OSAL_THREAD_OSAL_164,          0x1U },
    {OSAL_THREAD_OSAL_165,          0x1U },
    {OSAL_THREAD_OSAL_166,          0x1U },
    {OSAL_THREAD_OSAL_167,          0x1U },
    {OSAL_THREAD_OSAL_168,          0x1U },
    {OSAL_THREAD_OSAL_169,          0x1U },
    {OSAL_THREAD_OSAL_170,          0x1U },
    {OSAL_THREAD_OSAL_171,          0x1U },
    {OSAL_THREAD_OSAL_172,          0x1U },
    {OSAL_THREAD_OSAL_173,          0x1U },
    {OSAL_THREAD_OSAL_174,          0x1U },
    {OSAL_THREAD_OSAL_175,          0x1U },
    {OSAL_THREAD_OSAL_176,          0x1U },
    {OSAL_THREAD_OSAL_177,          0x1U },
    {OSAL_THREAD_OSAL_178,          0x1U },
    {OSAL_THREAD_OSAL_179,          0x1U },
    {OSAL_THREAD_OSAL_180,          0x1U },
    {OSAL_THREAD_OSAL_181,          0x1U },
    {OSAL_THREAD_OSAL_182,          0x1U },
    {OSAL_THREAD_OSAL_183,          0x1U },
    {OSAL_THREAD_OSAL_184,          0x1U },
    {OSAL_THREAD_OSAL_185,          0x1U },
    {OSAL_THREAD_OSAL_186,          0x1U },
    {OSAL_THREAD_OSAL_187,          0x1U },
    {OSAL_THREAD_OSAL_188,          0x1U },
    {OSAL_THREAD_OSAL_189,          0x1U },
    {OSAL_THREAD_OSAL_190,          0x1U },
    {OSAL_THREAD_OSAL_191,          0x1U },
    {OSAL_THREAD_OSAL_192,          0x1U },
    {OSAL_THREAD_OSAL_193,          0x1U },
    {OSAL_THREAD_OSAL_194,          0x1U },
    {OSAL_THREAD_OSAL_195,          0x1U },
    {OSAL_THREAD_OSAL_196,          0x1U },
    {OSAL_THREAD_OSAL_197,          0x1U },
    {OSAL_THREAD_OSAL_198,          0x1U },
    {OSAL_THREAD_OSAL_199,          0x1U },
    {OSAL_THREAD_OSAL_200,          0x1U },
    {OSAL_THREAD_OSAL_201,          0x1U },
    {OSAL_THREAD_OSAL_202,          0x1U },
    {OSAL_THREAD_OSAL_203,          0x1U },
    {OSAL_THREAD_OSAL_204,          0x1U },
    {OSAL_THREAD_OSAL_205,          0x1U },
    {OSAL_THREAD_OSAL_206,          0x1U },
    {OSAL_THREAD_OSAL_207,          0x1U },
    {OSAL_THREAD_OSAL_208,          0x1U },
    {OSAL_THREAD_OSAL_209,          0x1U },
    {OSAL_THREAD_OSAL_210,          0x1U },
    {OSAL_THREAD_OSAL_211,          0x1U },
    {OSAL_THREAD_OSAL_212,          0x1U },
    {OSAL_THREAD_OSAL_213,          0x1U },
    {OSAL_THREAD_OSAL_214,          0x1U },
    {OSAL_THREAD_OSAL_215,          0x1U },
    {OSAL_THREAD_OSAL_216,          0x1U },
    {OSAL_THREAD_OSAL_217,          0x1U },
    {OSAL_THREAD_OSAL_218,          0x1U },
    {OSAL_THREAD_OSAL_219,          0x1U },
    {OSAL_THREAD_OSAL_220,          0x1U },
    {OSAL_THREAD_OSAL_221,          0x1U },
    {OSAL_THREAD_OSAL_222,          0x1U },
    {OSAL_THREAD_OSAL_223,          0x1U },
    {OSAL_THREAD_OSAL_224,          0x1U },
    {OSAL_THREAD_OSAL_225,          0x1U },
    {OSAL_THREAD_OSAL_226,          0x1U },
    {OSAL_THREAD_OSAL_227,          0x1U },
    {OSAL_THREAD_OSAL_228,          0x1U },
    {OSAL_THREAD_OSAL_229,          0x1U },
    {OSAL_THREAD_OSAL_230,          0x1U },
    {OSAL_THREAD_OSAL_231,          0x1U },
    {OSAL_THREAD_OSAL_232,          0x1U },
    {OSAL_THREAD_OSAL_233,          0x1U },
    {OSAL_THREAD_OSAL_234,          0x1U },
    {OSAL_THREAD_OSAL_235,          0x1U },
    {OSAL_THREAD_OSAL_236,          0x1U },
    {OSAL_THREAD_OSAL_237,          0x1U },
    {OSAL_THREAD_OSAL_238,          0x1U },
    {OSAL_THREAD_OSAL_239,          0x1U },
    {OSAL_THREAD_OSAL_240,          0x1U },
    {OSAL_THREAD_OSAL_241,          0x1U },
    {OSAL_THREAD_OSAL_242,          0x1U },
    {OSAL_THREAD_OSAL_243,          0x1U },
    {OSAL_THREAD_OSAL_244,          0x1U },
    {OSAL_THREAD_OSAL_245,          0x1U },
    {OSAL_THREAD_OSAL_246,          0x1U },
    {OSAL_THREAD_OSAL_247,          0x1U },
    {OSAL_THREAD_OSAL_248,          0x1U },
    {OSAL_THREAD_OSAL_249,          0x1U },
    {OSAL_THREAD_OSAL_250,          0x1U },
    {OSAL_THREAD_OSAL_251,          0x1U },
    {OSAL_THREAD_OSAL_252,          0x1U },
    {OSAL_THREAD_OSAL_253,          0x1U },
    {OSAL_THREAD_OSAL_254,          0x1U },
    {OSAL_THREAD_OSAL_255,          0x1U },
    {OSAL_THREAD_OSAL_256,          0x1U },
    {OSAL_THREAD_OSAL_257,          0x1U },
    {OSAL_THREAD_OSAL_258,          0x1U },
    {OSAL_THREAD_OSAL_259,          0x1U },
    {OSAL_THREAD_OSAL_260,          0x1U },
    {OSAL_THREAD_OSAL_261,          0x1U },
    {OSAL_THREAD_OSAL_262,          0x1U },
    {OSAL_THREAD_OSAL_263,          0x1U },
    {OSAL_THREAD_OSAL_264,          0x1U },
    {OSAL_THREAD_OSAL_265,          0x1U },
    {OSAL_THREAD_OSAL_266,          0x1U },
    {OSAL_THREAD_OSAL_267,          0x1U },
    {OSAL_THREAD_OSAL_268,          0x1U },
    {OSAL_THREAD_OSAL_269,          0x1U },
    {OSAL_THREAD_OSAL_270,          0x1U },
    {OSAL_THREAD_OSAL_271,          0x1U },
    {OSAL_THREAD_OSAL_272,          0x1U },
    {OSAL_THREAD_OSAL_273,          0x1U },
    {OSAL_THREAD_OSAL_274,          0x1U },
    {OSAL_THREAD_OSAL_275,          0x1U },
    {OSAL_THREAD_OSAL_276,          0x1U },
    {OSAL_THREAD_OSAL_277,          0x1U },
    {OSAL_THREAD_OSAL_278,          0x1U },
    {OSAL_THREAD_OSAL_279,          0x1U },
    {OSAL_THREAD_OSAL_280,          0x1U },
    {OSAL_THREAD_OSAL_281,          0x1U },
    {OSAL_THREAD_OSAL_282,          0x1U },
    {OSAL_THREAD_OSAL_283,          0x1U },
    {OSAL_THREAD_OSAL_284,          0x1U },
    {OSAL_THREAD_OSAL_285,          0x1U },
    {OSAL_THREAD_OSAL_286,          0x1U },
    {OSAL_THREAD_OSAL_287,          0x1U },
    {OSAL_THREAD_OSAL_288,          0x1U },
    {OSAL_THREAD_OSAL_289,          0x1U },
    {OSAL_THREAD_OSAL_290,          0x1U },
    {OSAL_THREAD_OSAL_291,          0x1U },
    {OSAL_THREAD_OSAL_292,          0x1U },
    {OSAL_THREAD_OSAL_293,          0x1U },
    {OSAL_THREAD_OSAL_294,          0x1U },
    {OSAL_THREAD_OSAL_295,          0x1U },
    {OSAL_THREAD_OSAL_296,          0x1U },
    {OSAL_THREAD_OSAL_297,          0x1U },
    {OSAL_THREAD_OSAL_298,          0x1U },
    {OSAL_THREAD_OSAL_299,          0x1U },
    {OSAL_THREAD_OSAL_300,          0x1U },
    {OSAL_THREAD_OSAL_301,          0x1U },
    {OSAL_THREAD_OSAL_302,          0x1U },
    {OSAL_THREAD_OSAL_303,          0x1U },
    {OSAL_THREAD_OSAL_304,          0x1U },
    {OSAL_THREAD_OSAL_305,          0x1U },
    {OSAL_THREAD_OSAL_306,          0x1U },
    {OSAL_THREAD_OSAL_307,          0x1U },
    {OSAL_THREAD_OSAL_308,          0x1U },
    {OSAL_THREAD_OSAL_309,          0x1U },
    {OSAL_THREAD_OSAL_310,          0x1U },
    {OSAL_THREAD_OSAL_311,          0x1U },
    {OSAL_THREAD_OSAL_312,          0x1U },
    {OSAL_THREAD_OSAL_313,          0x1U },
    {OSAL_THREAD_OSAL_314,          0x1U },
    {OSAL_THREAD_OSAL_315,          0x1U },
    {OSAL_THREAD_OSAL_316,          0x1U },
    {OSAL_THREAD_OSAL_317,          0x1U },
    {OSAL_THREAD_OSAL_318,          0x1U },
    {OSAL_THREAD_OSAL_319,          0x1U },
    {OSAL_THREAD_OSAL_320,          0x1U },
    {OSAL_THREAD_OSAL_321,          0x1U },
    {OSAL_THREAD_OSAL_322,          0x1U },
    {OSAL_THREAD_OSAL_323,          0x1U },
    {OSAL_THREAD_OSAL_324,          0x1U },
    {OSAL_THREAD_OSAL_325,          0x1U },
    {OSAL_THREAD_OSAL_326,          0x1U },
    {OSAL_THREAD_OSAL_327,          0x1U },
    {OSAL_THREAD_OSAL_328,          0x1U },
    {OSAL_THREAD_OSAL_329,          0x1U },
    {OSAL_THREAD_OSAL_330,          0x1U },
    {OSAL_THREAD_OSAL_331,          0x1U },
    {OSAL_THREAD_OSAL_332,          0x1U },
    {OSAL_THREAD_OSAL_333,          0x1U },
    {OSAL_THREAD_OSAL_334,          0x1U },
    {OSAL_THREAD_OSAL_335,          0x1U },
    {OSAL_THREAD_OSAL_336,          0x1U },
    {OSAL_THREAD_OSAL_337,          0x1U },
    {OSAL_THREAD_OSAL_338,          0x1U },
    {OSAL_THREAD_OSAL_339,          0x1U },
    {OSAL_THREAD_OSAL_340,          0x1U },
    {OSAL_THREAD_OSAL_341,          0x1U },
    {OSAL_THREAD_OSAL_342,          0x1U },
    {OSAL_THREAD_OSAL_343,          0x1U },
    {OSAL_THREAD_OSAL_344,          0x1U },
    {OSAL_THREAD_OSAL_345,          0x1U },
    {OSAL_THREAD_OSAL_346,          0x1U },
    {OSAL_THREAD_OSAL_347,          0x1U },
    {OSAL_THREAD_OSAL_348,          0x1U },
    {OSAL_THREAD_OSAL_349,          0x1U },
    {OSAL_THREAD_OSAL_350,          0x1U },
    {OSAL_THREAD_OSAL_351,          0x1U },
    {OSAL_THREAD_OSAL_352,          0x1U },
    {OSAL_THREAD_OSAL_353,          0x1U },
    {OSAL_THREAD_OSAL_354,          0x1U },
    {OSAL_THREAD_OSAL_355,          0x1U },
    {OSAL_THREAD_OSAL_356,          0x1U },
    {OSAL_THREAD_OSAL_357,          0x1U },
    {OSAL_THREAD_OSAL_358,          0x1U },
    {OSAL_THREAD_OSAL_359,          0x1U },
    {OSAL_THREAD_OSAL_360,          0x1U },
    {OSAL_THREAD_OSAL_361,          0x1U },
    {OSAL_THREAD_OSAL_362,          0x1U },
    {OSAL_THREAD_OSAL_363,          0x1U },
    {OSAL_THREAD_OSAL_364,          0x1U },
    {OSAL_THREAD_OSAL_365,          0x1U },
    {OSAL_THREAD_OSAL_366,          0x1U },
    {OSAL_THREAD_OSAL_367,          0x1U },
    {OSAL_THREAD_OSAL_368,          0x1U },
    {OSAL_THREAD_OSAL_369,          0x1U },
    {OSAL_THREAD_OSAL_370,          0x1U },
    {OSAL_THREAD_OSAL_371,          0x1U },
    {OSAL_THREAD_OSAL_372,          0x1U },
    {OSAL_THREAD_OSAL_373,          0x1U },
    {OSAL_THREAD_OSAL_374,          0x1U },
    {OSAL_THREAD_OSAL_375,          0x1U },
    {OSAL_THREAD_OSAL_376,          0x1U },
    {OSAL_THREAD_OSAL_377,          0x1U },
    {OSAL_THREAD_OSAL_378,          0x1U },
    {OSAL_THREAD_OSAL_379,          0x1U },
    {OSAL_THREAD_OSAL_380,          0x1U },
    {OSAL_THREAD_OSAL_381,          0x1U },
    {OSAL_THREAD_OSAL_382,          0x1U },
    {OSAL_THREAD_OSAL_383,          0x1U },
    {OSAL_THREAD_OSAL_384,          0x1U },
    {OSAL_THREAD_OSAL_385,          0x1U },
    {OSAL_THREAD_OSAL_386,          0x1U },
    {OSAL_THREAD_OSAL_387,          0x1U },
    {OSAL_THREAD_OSAL_388,          0x1U },
    {OSAL_THREAD_OSAL_389,          0x1U },
    {OSAL_THREAD_OSAL_390,          0x1U },
    {OSAL_THREAD_OSAL_391,          0x1U },
    {OSAL_THREAD_OSAL_392,          0x1U },
    {OSAL_THREAD_OSAL_393,          0x1U },
    {OSAL_THREAD_OSAL_394,          0x1U },
    {OSAL_THREAD_OSAL_395,          0x1U },
    {OSAL_THREAD_OSAL_396,          0x1U },
    {OSAL_THREAD_OSAL_397,          0x1U },
    {OSAL_THREAD_OSAL_398,          0x1U },
    {OSAL_THREAD_OSAL_399,          0x1U },
    {OSAL_THREAD_OSAL_400,          0x1U },
    {OSAL_THREAD_OSAL_401,          0x1U },
    {OSAL_THREAD_OSAL_402,          0x1U },
    {OSAL_THREAD_OSAL_403,          0x1U },
    {OSAL_THREAD_OSAL_404,          0x1U },
    {OSAL_THREAD_OSAL_405,          0x1U },
    {OSAL_THREAD_OSAL_406,          0x1U },
    {OSAL_THREAD_OSAL_407,          0x1U },
    {OSAL_THREAD_OSAL_408,          0x1U },
    {OSAL_THREAD_OSAL_409,          0x1U },
    {OSAL_THREAD_OSAL_410,          0x1U },
    {OSAL_THREAD_OSAL_411,          0x1U },
    {OSAL_THREAD_OSAL_412,          0x1U },
    {OSAL_THREAD_OSAL_413,          0x1U },
    {OSAL_THREAD_OSAL_414,          0x1U },
    {OSAL_THREAD_OSAL_415,          0x1U },
    {OSAL_THREAD_OSAL_416,          0x1U },
    {OSAL_THREAD_OSAL_417,          0x1U },
    {OSAL_THREAD_OSAL_418,          0x1U },
    {OSAL_THREAD_OSAL_419,          0x1U },
    {OSAL_THREAD_OSAL_420,          0x1U },
    {OSAL_THREAD_OSAL_421,          0x1U },
    {OSAL_THREAD_OSAL_422,          0x1U },
    {OSAL_THREAD_OSAL_423,          0x1U },
    {OSAL_THREAD_OSAL_424,          0x1U },
    {OSAL_THREAD_OSAL_425,          0x1U },
    {OSAL_THREAD_OSAL_426,          0x1U },
    {OSAL_THREAD_OSAL_427,          0x1U },
    {OSAL_THREAD_OSAL_428,          0x1U },
    {OSAL_THREAD_OSAL_429,          0x1U },
    {OSAL_THREAD_OSAL_430,          0x1U },
    {OSAL_THREAD_OSAL_431,          0x1U },
    {OSAL_THREAD_OSAL_432,          0x1U },
    {OSAL_THREAD_OSAL_433,          0x1U },
    {OSAL_THREAD_OSAL_434,          0x1U },
    {OSAL_THREAD_OSAL_435,          0x1U },
    {OSAL_THREAD_OSAL_436,          0x1U },
    {OSAL_THREAD_OSAL_437,          0x1U },
    {OSAL_THREAD_OSAL_438,          0x1U },
    {OSAL_THREAD_OSAL_439,          0x1U },
    {OSAL_THREAD_OSAL_440,          0x1U },
    {OSAL_THREAD_OSAL_441,          0x1U },
    {OSAL_THREAD_OSAL_442,          0x1U },
    {OSAL_THREAD_OSAL_443,          0x1U },
    {OSAL_THREAD_OSAL_444,          0x1U },
    {OSAL_THREAD_OSAL_445,          0x1U },
    {OSAL_THREAD_OSAL_446,          0x1U },
    {OSAL_THREAD_OSAL_447,          0x1U },
    {OSAL_THREAD_OSAL_448,          0x1U },
    {OSAL_THREAD_OSAL_449,          0x1U },
    {OSAL_THREAD_OSAL_450,          0x1U },
    {OSAL_THREAD_OSAL_451,          0x1U },
    {OSAL_THREAD_OSAL_452,          0x1U },
    {OSAL_THREAD_OSAL_453,          0x1U },
    {OSAL_THREAD_OSAL_454,          0x1U },
    {OSAL_THREAD_OSAL_455,          0x1U },
    {OSAL_THREAD_OSAL_456,          0x1U },
    {OSAL_THREAD_OSAL_457,          0x1U },
    {OSAL_THREAD_OSAL_458,          0x1U },
    {OSAL_THREAD_OSAL_459,          0x1U },
    {OSAL_THREAD_OSAL_460,          0x1U },
    {OSAL_THREAD_OSAL_461,          0x1U },
    {OSAL_THREAD_OSAL_462,          0x1U },
    {OSAL_THREAD_OSAL_463,          0x1U },
    {OSAL_THREAD_OSAL_464,          0x1U },
    {OSAL_THREAD_OSAL_465,          0x1U },
    {OSAL_THREAD_OSAL_466,          0x1U },
    {OSAL_THREAD_OSAL_467,          0x1U },
    {OSAL_THREAD_OSAL_468,          0x1U },
    {OSAL_THREAD_OSAL_469,          0x1U },
    {OSAL_THREAD_OSAL_470,          0x1U },
    {OSAL_THREAD_OSAL_471,          0x1U },
    {OSAL_THREAD_OSAL_472,          0x1U },
    {OSAL_THREAD_OSAL_473,          0x1U },
    {OSAL_THREAD_OSAL_474,          0x1U },
    {OSAL_THREAD_OSAL_475,          0x1U },
    {OSAL_THREAD_OSAL_476,          0x1U },
    {OSAL_THREAD_OSAL_477,          0x1U },
    {OSAL_THREAD_OSAL_478,          0x1U },
    {OSAL_THREAD_OSAL_479,          0x1U },
    {OSAL_THREAD_OSAL_480,          0x1U },
    {OSAL_THREAD_OSAL_481,          0x1U },
    {OSAL_THREAD_OSAL_482,          0x1U },
    {OSAL_THREAD_OSAL_483,          0x1U },
    {OSAL_THREAD_OSAL_484,          0x1U },
    {OSAL_THREAD_OSAL_485,          0x1U },
    {OSAL_THREAD_OSAL_486,          0x1U },
    {OSAL_THREAD_OSAL_487,          0x1U },
    {OSAL_THREAD_OSAL_488,          0x1U },
    {OSAL_THREAD_OSAL_489,          0x1U },
    {OSAL_THREAD_OSAL_490,          0x1U },
    {OSAL_THREAD_OSAL_491,          0x1U },
    {OSAL_THREAD_OSAL_492,          0x1U },
    {OSAL_THREAD_OSAL_493,          0x1U },
    {OSAL_THREAD_OSAL_494,          0x1U },
    {OSAL_THREAD_OSAL_495,          0x1U },
    {OSAL_THREAD_OSAL_496,          0x1U },
    {OSAL_THREAD_OSAL_497,          0x1U },
    {OSAL_THREAD_OSAL_498,          0x1U },
    {OSAL_THREAD_OSAL_499,          0x1U },
#endif
    {OSAL_THREAD_OSAL_PMA,          0x1U },

    {.id = OSAL_THREAD_AFFINITY_INVALID_ID}
};

/*******************************************************************************************************************//**
 * @var g_osal_interrupt_thread_affinity
 * @brief osal device configuration : CPU affinity value for Interrupt thread
***********************************************************************************************************************/
/* PRQA S 1531 1 # It must be external linkage separately from the referenced file because of the object to be exposed to the user. */
const st_osal_interrupt_thread_affinity_t  g_osal_interrupt_thread_affinity[] =
{
    /* All default CPU Affinity */
    {.irq_number = OSAL_INTERRUPT_AFFINITY_INVALID_ID}
};
#endif

/*******************************************************************************************************************//**
 * @var g_osal_thread_config
 * osal user configuration : thread configuration table
***********************************************************************************************************************/
#ifndef TEST_LINUX
const st_osal_rcfg_thread_config_t g_osal_thread_config[] =
{
    /* thread id,                   fucn, userarg, priority,                 task_name,          stack_size */
    /* xOS2 resource information */
    {OSAL_THREAD_OSAL_00,           {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task01",         2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_01,           {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task02",         2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_02,           {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task03",         2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_03,           {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task04",         2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_04,           {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task05",         2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_05,           {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task06",         2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_06,           {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task07",         2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_07,           {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task08",         2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_08,           {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task09",         2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_09,           {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task10",         2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_10,           {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task11",         2 * 1024 * 1024} },
#if defined(ENABLE_LONGTIME_TEST)
    {OSAL_THREAD_OSAL_11,           {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task12",         2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_12,           {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task13",         2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_13,           {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task14",         2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_14,           {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task15",         2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_15,           {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task16",         2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_16,           {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task17",         2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_17,           {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task18",         2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_18,           {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task19",         2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_19,           {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task20",         2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_20,           {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task21",         2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_21,           {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task22",         2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_22,           {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task23",         2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_23,           {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task24",         2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_24,           {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task25",         2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_25,           {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task26",         2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_26,           {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task27",         2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_27,           {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task28",         2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_28,           {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task29",         2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_29,           {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task30",         2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_30,           {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task31",         2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_31,           {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task32",         2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_32,           {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task33",         2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_33,           {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task34",         2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_34,           {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task35",         2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_35,           {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task36",         2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_36,           {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task37",         2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_37,           {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task38",         2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_38,           {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task39",         2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_39,           {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task40",         2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_40,           {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task41",         2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_41,           {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task42",         2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_42,           {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task43",         2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_43,           {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task44",         2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_44,           {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task45",         2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_45,           {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task46",         2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_46,           {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task47",         2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_47,           {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task48",         2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_48,           {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task49",         2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_49,           {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task50",         2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_50,           {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task51",         2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_51,           {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task52",         2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_52,           {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task53",         2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_53,           {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task54",         2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_54,           {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task55",         2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_55,           {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task56",         2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_56,           {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task57",         2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_57,           {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task58",         2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_58,           {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task59",         2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_59,           {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task60",         2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_60,           {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task61",         2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_61,           {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task62",         2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_62,           {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task63",         2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_63,           {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task64",         2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_64,           {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task65",         2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_65,           {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task66",         2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_66,           {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task67",         2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_67,           {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task68",         2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_68,           {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task69",         2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_69,           {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task70",         2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_70,           {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task71",         2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_71,           {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task72",         2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_72,           {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task73",         2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_73,           {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task74",         2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_74,           {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task75",         2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_75,           {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task76",         2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_76,           {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task77",         2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_77,           {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task78",         2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_78,           {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task79",         2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_79,           {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task80",         2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_80,           {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task81",         2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_81,           {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task82",         2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_82,           {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task83",         2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_83,           {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task84",         2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_84,           {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task85",         2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_85,           {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task86",         2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_86,           {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task87",         2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_87,           {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task88",         2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_88,           {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task89",         2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_89,           {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task90",         2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_90,           {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task91",         2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_91,           {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task92",         2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_92,           {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task93",         2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_93,           {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task94",         2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_94,           {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task95",         2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_95,           {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task96",         2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_96,           {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task97",         2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_97,           {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task98",         2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_98,           {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task99",         2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_99,           {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task100",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_100,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task101",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_101,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task102",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_102,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task103",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_103,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task104",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_104,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task105",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_105,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task106",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_106,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task107",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_107,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task108",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_108,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task109",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_109,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task110",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_110,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task111",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_111,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task112",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_112,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task113",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_113,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task114",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_114,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task115",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_115,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task116",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_116,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task117",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_117,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task118",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_118,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task119",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_119,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task120",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_120,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task121",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_121,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task122",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_122,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task123",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_123,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task124",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_124,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task125",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_125,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task126",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_126,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task127",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_127,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task128",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_128,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task129",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_129,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task130",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_130,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task131",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_131,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task132",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_132,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task133",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_133,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task134",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_134,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task135",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_135,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task136",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_136,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task137",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_137,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task138",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_138,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task139",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_139,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task140",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_140,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task141",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_141,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task142",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_142,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task143",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_143,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task144",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_144,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task145",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_145,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task146",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_146,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task147",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_147,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task148",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_148,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task149",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_149,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task150",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_150,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task151",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_151,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task152",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_152,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task153",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_153,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task154",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_154,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task155",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_155,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task156",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_156,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task157",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_157,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task158",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_158,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task159",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_159,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task160",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_160,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task161",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_161,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task162",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_162,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task163",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_163,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task164",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_164,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task165",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_165,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task166",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_166,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task167",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_167,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task168",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_168,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task169",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_169,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task170",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_170,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task171",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_171,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task172",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_172,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task173",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_173,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task174",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_174,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task175",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_175,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task176",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_176,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task177",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_177,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task178",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_178,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task179",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_179,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task180",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_180,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task181",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_181,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task182",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_182,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task183",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_183,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task184",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_184,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task185",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_185,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task186",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_186,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task187",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_187,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task188",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_188,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task189",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_189,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task190",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_190,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task191",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_191,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task192",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_192,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task193",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_193,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task194",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_194,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task195",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_195,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task196",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_196,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task197",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_197,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task198",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_198,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task199",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_199,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task200",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_200,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task201",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_201,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task202",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_202,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task203",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_203,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task204",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_204,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task205",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_205,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task206",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_206,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task207",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_207,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task208",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_208,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task209",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_209,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task210",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_210,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task211",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_211,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task212",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_212,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task213",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_213,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task214",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_214,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task215",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_215,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task216",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_216,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task217",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_217,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task218",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_218,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task219",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_219,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task220",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_220,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task221",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_221,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task222",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_222,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task223",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_223,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task224",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_224,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task225",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_225,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task226",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_226,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task227",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_227,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task228",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_228,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task229",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_229,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task230",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_230,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task231",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_231,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task232",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_232,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task233",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_233,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task234",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_234,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task235",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_235,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task236",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_236,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task237",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_237,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task238",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_238,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task239",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_239,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task240",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_240,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task241",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_241,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task242",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_242,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task243",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_243,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task244",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_244,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task245",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_245,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task246",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_246,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task247",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_247,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task248",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_248,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task249",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_249,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task250",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_250,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task251",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_251,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task252",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_252,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task253",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_253,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task254",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_254,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task255",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_255,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task256",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_256,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task257",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_257,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task258",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_258,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task259",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_259,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task260",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_260,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task261",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_261,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task262",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_262,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task263",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_263,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task264",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_264,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task265",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_265,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task266",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_266,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task267",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_267,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task268",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_268,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task269",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_269,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task270",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_270,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task271",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_271,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task272",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_272,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task273",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_273,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task274",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_274,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task275",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_275,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task276",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_276,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task277",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_277,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task278",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_278,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task279",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_279,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task280",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_280,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task281",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_281,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task282",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_282,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task283",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_283,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task284",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_284,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task285",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_285,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task286",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_286,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task287",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_287,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task288",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_288,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task289",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_289,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task290",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_290,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task291",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_291,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task292",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_292,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task293",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_293,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task294",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_294,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task295",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_295,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task296",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_296,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task297",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_297,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task298",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_298,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task299",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_299,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task300",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_300,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task301",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_301,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task302",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_302,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task303",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_303,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task304",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_304,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task305",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_305,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task306",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_306,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task307",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_307,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task308",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_308,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task309",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_309,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task310",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_310,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task311",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_311,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task312",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_312,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task313",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_313,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task314",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_314,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task315",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_315,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task316",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_316,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task317",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_317,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task318",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_318,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task319",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_319,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task320",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_320,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task321",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_321,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task322",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_322,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task323",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_323,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task324",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_324,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task325",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_325,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task326",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_326,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task327",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_327,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task328",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_328,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task329",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_329,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task330",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_330,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task331",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_331,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task332",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_332,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task333",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_333,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task334",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_334,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task335",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_335,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task336",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_336,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task337",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_337,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task338",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_338,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task339",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_339,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task340",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_340,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task341",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_341,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task342",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_342,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task343",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_343,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task344",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_344,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task345",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_345,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task346",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_346,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task347",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_347,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task348",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_348,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task349",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_349,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task350",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_350,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task351",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_351,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task352",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_352,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task353",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_353,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task354",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_354,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task355",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_355,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task356",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_356,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task357",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_357,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task358",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_358,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task359",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_359,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task360",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_360,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task361",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_361,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task362",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_362,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task363",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_363,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task364",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_364,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task365",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_365,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task366",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_366,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task367",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_367,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task368",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_368,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task369",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_369,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task370",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_370,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task371",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_371,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task372",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_372,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task373",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_373,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task374",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_374,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task375",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_375,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task376",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_376,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task377",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_377,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task378",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_378,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task379",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_379,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task380",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_380,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task381",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_381,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task382",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_382,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task383",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_383,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task384",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_384,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task385",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_385,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task386",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_386,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task387",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_387,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task388",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_388,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task389",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_389,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task390",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_390,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task391",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_391,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task392",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_392,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task393",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_393,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task394",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_394,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task395",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_395,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task396",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_396,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task397",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_397,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task398",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_398,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task399",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_399,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task400",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_400,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task401",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_401,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task402",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_402,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task403",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_403,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task404",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_404,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task405",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_405,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task406",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_406,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task407",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_407,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task408",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_408,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task409",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_409,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task410",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_410,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task411",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_411,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task412",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_412,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task413",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_413,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task414",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_414,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task415",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_415,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task416",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_416,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task417",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_417,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task418",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_418,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task419",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_419,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task420",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_420,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task421",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_421,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task422",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_422,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task423",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_423,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task424",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_424,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task425",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_425,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task426",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_426,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task427",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_427,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task428",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_428,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task429",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_429,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task430",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_430,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task431",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_431,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task432",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_432,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task433",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_433,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task434",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_434,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task435",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_435,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task436",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_436,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task437",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_437,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task438",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_438,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task439",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_439,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task440",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_440,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task441",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_441,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task442",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_442,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task443",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_443,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task444",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_444,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task445",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_445,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task446",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_446,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task447",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_447,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task448",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_448,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task449",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_449,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task450",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_450,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task451",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_451,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task452",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_452,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task453",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_453,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task454",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_454,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task455",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_455,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task456",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_456,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task457",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_457,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task458",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_458,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task459",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_459,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task460",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_460,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task461",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_461,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task462",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_462,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task463",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_463,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task464",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_464,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task465",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_465,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task466",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_466,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task467",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_467,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task468",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_468,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task469",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_469,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task470",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_470,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task471",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_471,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task472",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_472,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task473",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_473,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task474",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_474,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task475",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_475,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task476",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_476,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task477",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_477,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task478",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_478,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task479",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_479,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task480",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_480,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task481",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_481,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task482",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_482,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task483",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_483,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task484",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_484,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task485",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_485,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task486",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_486,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task487",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_487,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task488",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_488,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task489",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_489,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task490",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_490,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task491",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_491,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task492",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_492,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task493",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_493,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task494",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_494,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task495",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_495,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task496",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_496,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task497",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_497,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task498",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_498,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task499",        2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_499,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "osal_task500",        2 * 1024 * 1024} },
#endif
    /* tentative. internal reserve */
    {OSAL_THREAD_OSAL_PMA,          {NULL, NULL, OSAL_THREAD_PRIORITY_LOWEST, "R_PMA_SM_Task",      0x1000} },

    {.id = OSAL_THREAD_INVALID_ID}
};
#endif

/*******************************************************************************************************************//**
 * @var g_osal_mutex_config
 * osal user configuration : mutex configuration table
***********************************************************************************************************************/
#ifndef TEST_LINUX
const st_osal_rcfg_mutex_config_t g_osal_mutex_config[] =
{
    /* mutex id               */
    /* xOS2 resource information */
    {OSAL_MUTEX_OSAL_00        },
    {OSAL_MUTEX_OSAL_01        },
    {OSAL_MUTEX_OSAL_02        },
    {OSAL_MUTEX_OSAL_03        },
    {OSAL_MUTEX_OSAL_04        },
    {OSAL_MUTEX_OSAL_05        },
    {OSAL_MUTEX_OSAL_06        },
    {OSAL_MUTEX_OSAL_07        },
    {OSAL_MUTEX_OSAL_08        },
    {OSAL_MUTEX_OSAL_09        },
    {OSAL_MUTEX_OSAL_10        },
#if defined(ENABLE_LONGTIME_TEST)
    {OSAL_MUTEX_OSAL_11        },
    {OSAL_MUTEX_OSAL_12        },
    {OSAL_MUTEX_OSAL_13        },
    {OSAL_MUTEX_OSAL_14        },
    {OSAL_MUTEX_OSAL_15        },
    {OSAL_MUTEX_OSAL_16        },
    {OSAL_MUTEX_OSAL_17        },
    {OSAL_MUTEX_OSAL_18        },
    {OSAL_MUTEX_OSAL_19        },
    {OSAL_MUTEX_OSAL_20        },
    {OSAL_MUTEX_OSAL_21        },
    {OSAL_MUTEX_OSAL_22        },
    {OSAL_MUTEX_OSAL_23        },
    {OSAL_MUTEX_OSAL_24        },
    {OSAL_MUTEX_OSAL_25        },
    {OSAL_MUTEX_OSAL_26        },
    {OSAL_MUTEX_OSAL_27        },
    {OSAL_MUTEX_OSAL_28        },
    {OSAL_MUTEX_OSAL_29        },
    {OSAL_MUTEX_OSAL_30        },
    {OSAL_MUTEX_OSAL_31        },
    {OSAL_MUTEX_OSAL_32        },
    {OSAL_MUTEX_OSAL_33        },
    {OSAL_MUTEX_OSAL_34        },
    {OSAL_MUTEX_OSAL_35        },
    {OSAL_MUTEX_OSAL_36        },
    {OSAL_MUTEX_OSAL_37        },
    {OSAL_MUTEX_OSAL_38        },
    {OSAL_MUTEX_OSAL_39        },
    {OSAL_MUTEX_OSAL_40        },
    {OSAL_MUTEX_OSAL_41        },
    {OSAL_MUTEX_OSAL_42        },
    {OSAL_MUTEX_OSAL_43        },
    {OSAL_MUTEX_OSAL_44        },
    {OSAL_MUTEX_OSAL_45        },
    {OSAL_MUTEX_OSAL_46        },
    {OSAL_MUTEX_OSAL_47        },
    {OSAL_MUTEX_OSAL_48        },
    {OSAL_MUTEX_OSAL_49        },
    {OSAL_MUTEX_OSAL_50        },
    {OSAL_MUTEX_OSAL_51        },
    {OSAL_MUTEX_OSAL_52        },
    {OSAL_MUTEX_OSAL_53        },
    {OSAL_MUTEX_OSAL_54        },
    {OSAL_MUTEX_OSAL_55        },
    {OSAL_MUTEX_OSAL_56        },
    {OSAL_MUTEX_OSAL_57        },
    {OSAL_MUTEX_OSAL_58        },
    {OSAL_MUTEX_OSAL_59        },
    {OSAL_MUTEX_OSAL_60        },
    {OSAL_MUTEX_OSAL_61        },
    {OSAL_MUTEX_OSAL_62        },
    {OSAL_MUTEX_OSAL_63        },
    {OSAL_MUTEX_OSAL_64        },
    {OSAL_MUTEX_OSAL_65        },
    {OSAL_MUTEX_OSAL_66        },
    {OSAL_MUTEX_OSAL_67        },
    {OSAL_MUTEX_OSAL_68        },
    {OSAL_MUTEX_OSAL_69        },
    {OSAL_MUTEX_OSAL_70        },
    {OSAL_MUTEX_OSAL_71        },
    {OSAL_MUTEX_OSAL_72        },
    {OSAL_MUTEX_OSAL_73        },
    {OSAL_MUTEX_OSAL_74        },
    {OSAL_MUTEX_OSAL_75        },
    {OSAL_MUTEX_OSAL_76        },
    {OSAL_MUTEX_OSAL_77        },
    {OSAL_MUTEX_OSAL_78        },
    {OSAL_MUTEX_OSAL_79        },
    {OSAL_MUTEX_OSAL_80        },
    {OSAL_MUTEX_OSAL_81        },
    {OSAL_MUTEX_OSAL_82        },
    {OSAL_MUTEX_OSAL_83        },
    {OSAL_MUTEX_OSAL_84        },
    {OSAL_MUTEX_OSAL_85        },
    {OSAL_MUTEX_OSAL_86        },
    {OSAL_MUTEX_OSAL_87        },
    {OSAL_MUTEX_OSAL_88        },
    {OSAL_MUTEX_OSAL_89        },
    {OSAL_MUTEX_OSAL_90        },
    {OSAL_MUTEX_OSAL_91        },
    {OSAL_MUTEX_OSAL_92        },
    {OSAL_MUTEX_OSAL_93        },
    {OSAL_MUTEX_OSAL_94        },
    {OSAL_MUTEX_OSAL_95        },
    {OSAL_MUTEX_OSAL_96        },
    {OSAL_MUTEX_OSAL_97        },
    {OSAL_MUTEX_OSAL_98        },
    {OSAL_MUTEX_OSAL_99        },
    {OSAL_MUTEX_OSAL_100        },
    {OSAL_MUTEX_OSAL_101        },
    {OSAL_MUTEX_OSAL_102        },
    {OSAL_MUTEX_OSAL_103        },
    {OSAL_MUTEX_OSAL_104        },
    {OSAL_MUTEX_OSAL_105        },
    {OSAL_MUTEX_OSAL_106        },
    {OSAL_MUTEX_OSAL_107        },
    {OSAL_MUTEX_OSAL_108        },
    {OSAL_MUTEX_OSAL_109        },
    {OSAL_MUTEX_OSAL_110        },
    {OSAL_MUTEX_OSAL_111        },
    {OSAL_MUTEX_OSAL_112        },
    {OSAL_MUTEX_OSAL_113        },
    {OSAL_MUTEX_OSAL_114        },
    {OSAL_MUTEX_OSAL_115        },
    {OSAL_MUTEX_OSAL_116        },
    {OSAL_MUTEX_OSAL_117        },
    {OSAL_MUTEX_OSAL_118        },
    {OSAL_MUTEX_OSAL_119        },
    {OSAL_MUTEX_OSAL_120        },
    {OSAL_MUTEX_OSAL_121        },
    {OSAL_MUTEX_OSAL_122        },
    {OSAL_MUTEX_OSAL_123        },
    {OSAL_MUTEX_OSAL_124        },
    {OSAL_MUTEX_OSAL_125        },
    {OSAL_MUTEX_OSAL_126        },
    {OSAL_MUTEX_OSAL_127        },
    {OSAL_MUTEX_OSAL_128        },
    {OSAL_MUTEX_OSAL_129        },
    {OSAL_MUTEX_OSAL_130        },
    {OSAL_MUTEX_OSAL_131        },
    {OSAL_MUTEX_OSAL_132        },
    {OSAL_MUTEX_OSAL_133        },
    {OSAL_MUTEX_OSAL_134        },
    {OSAL_MUTEX_OSAL_135        },
    {OSAL_MUTEX_OSAL_136        },
    {OSAL_MUTEX_OSAL_137        },
    {OSAL_MUTEX_OSAL_138        },
    {OSAL_MUTEX_OSAL_139        },
    {OSAL_MUTEX_OSAL_140        },
    {OSAL_MUTEX_OSAL_141        },
    {OSAL_MUTEX_OSAL_142        },
    {OSAL_MUTEX_OSAL_143        },
    {OSAL_MUTEX_OSAL_144        },
    {OSAL_MUTEX_OSAL_145        },
    {OSAL_MUTEX_OSAL_146        },
    {OSAL_MUTEX_OSAL_147        },
    {OSAL_MUTEX_OSAL_148        },
    {OSAL_MUTEX_OSAL_149        },
    {OSAL_MUTEX_OSAL_150        },
    {OSAL_MUTEX_OSAL_151        },
    {OSAL_MUTEX_OSAL_152        },
    {OSAL_MUTEX_OSAL_153        },
    {OSAL_MUTEX_OSAL_154        },
    {OSAL_MUTEX_OSAL_155        },
    {OSAL_MUTEX_OSAL_156        },
    {OSAL_MUTEX_OSAL_157        },
    {OSAL_MUTEX_OSAL_158        },
    {OSAL_MUTEX_OSAL_159        },
    {OSAL_MUTEX_OSAL_160        },
    {OSAL_MUTEX_OSAL_161        },
    {OSAL_MUTEX_OSAL_162        },
    {OSAL_MUTEX_OSAL_163        },
    {OSAL_MUTEX_OSAL_164        },
    {OSAL_MUTEX_OSAL_165        },
    {OSAL_MUTEX_OSAL_166        },
    {OSAL_MUTEX_OSAL_167        },
    {OSAL_MUTEX_OSAL_168        },
    {OSAL_MUTEX_OSAL_169        },
    {OSAL_MUTEX_OSAL_170        },
    {OSAL_MUTEX_OSAL_171        },
    {OSAL_MUTEX_OSAL_172        },
    {OSAL_MUTEX_OSAL_173        },
    {OSAL_MUTEX_OSAL_174        },
    {OSAL_MUTEX_OSAL_175        },
    {OSAL_MUTEX_OSAL_176        },
    {OSAL_MUTEX_OSAL_177        },
    {OSAL_MUTEX_OSAL_178        },
    {OSAL_MUTEX_OSAL_179        },
    {OSAL_MUTEX_OSAL_180        },
    {OSAL_MUTEX_OSAL_181        },
    {OSAL_MUTEX_OSAL_182        },
    {OSAL_MUTEX_OSAL_183        },
    {OSAL_MUTEX_OSAL_184        },
    {OSAL_MUTEX_OSAL_185        },
    {OSAL_MUTEX_OSAL_186        },
    {OSAL_MUTEX_OSAL_187        },
    {OSAL_MUTEX_OSAL_188        },
    {OSAL_MUTEX_OSAL_189        },
    {OSAL_MUTEX_OSAL_190        },
    {OSAL_MUTEX_OSAL_191        },
    {OSAL_MUTEX_OSAL_192        },
    {OSAL_MUTEX_OSAL_193        },
    {OSAL_MUTEX_OSAL_194        },
    {OSAL_MUTEX_OSAL_195        },
    {OSAL_MUTEX_OSAL_196        },
    {OSAL_MUTEX_OSAL_197        },
    {OSAL_MUTEX_OSAL_198        },
    {OSAL_MUTEX_OSAL_199        },
    {OSAL_MUTEX_OSAL_200        },
    {OSAL_MUTEX_OSAL_201        },
    {OSAL_MUTEX_OSAL_202        },
    {OSAL_MUTEX_OSAL_203        },
    {OSAL_MUTEX_OSAL_204        },
    {OSAL_MUTEX_OSAL_205        },
    {OSAL_MUTEX_OSAL_206        },
    {OSAL_MUTEX_OSAL_207        },
    {OSAL_MUTEX_OSAL_208        },
    {OSAL_MUTEX_OSAL_209        },
    {OSAL_MUTEX_OSAL_210        },
    {OSAL_MUTEX_OSAL_211        },
    {OSAL_MUTEX_OSAL_212        },
    {OSAL_MUTEX_OSAL_213        },
    {OSAL_MUTEX_OSAL_214        },
    {OSAL_MUTEX_OSAL_215        },
    {OSAL_MUTEX_OSAL_216        },
    {OSAL_MUTEX_OSAL_217        },
    {OSAL_MUTEX_OSAL_218        },
    {OSAL_MUTEX_OSAL_219        },
    {OSAL_MUTEX_OSAL_220        },
    {OSAL_MUTEX_OSAL_221        },
    {OSAL_MUTEX_OSAL_222        },
    {OSAL_MUTEX_OSAL_223        },
    {OSAL_MUTEX_OSAL_224        },
    {OSAL_MUTEX_OSAL_225        },
    {OSAL_MUTEX_OSAL_226        },
    {OSAL_MUTEX_OSAL_227        },
    {OSAL_MUTEX_OSAL_228        },
    {OSAL_MUTEX_OSAL_229        },
    {OSAL_MUTEX_OSAL_230        },
    {OSAL_MUTEX_OSAL_231        },
    {OSAL_MUTEX_OSAL_232        },
    {OSAL_MUTEX_OSAL_233        },
    {OSAL_MUTEX_OSAL_234        },
    {OSAL_MUTEX_OSAL_235        },
    {OSAL_MUTEX_OSAL_236        },
    {OSAL_MUTEX_OSAL_237        },
    {OSAL_MUTEX_OSAL_238        },
    {OSAL_MUTEX_OSAL_239        },
    {OSAL_MUTEX_OSAL_240        },
    {OSAL_MUTEX_OSAL_241        },
    {OSAL_MUTEX_OSAL_242        },
    {OSAL_MUTEX_OSAL_243        },
    {OSAL_MUTEX_OSAL_244        },
    {OSAL_MUTEX_OSAL_245        },
    {OSAL_MUTEX_OSAL_246        },
    {OSAL_MUTEX_OSAL_247        },
    {OSAL_MUTEX_OSAL_248        },
    {OSAL_MUTEX_OSAL_249        },
    {OSAL_MUTEX_OSAL_250        },
    {OSAL_MUTEX_OSAL_251        },
    {OSAL_MUTEX_OSAL_252        },
    {OSAL_MUTEX_OSAL_253        },
    {OSAL_MUTEX_OSAL_254        },
    {OSAL_MUTEX_OSAL_255        },
    {OSAL_MUTEX_OSAL_256        },
    {OSAL_MUTEX_OSAL_257        },
    {OSAL_MUTEX_OSAL_258        },
    {OSAL_MUTEX_OSAL_259        },
    {OSAL_MUTEX_OSAL_260        },
    {OSAL_MUTEX_OSAL_261        },
    {OSAL_MUTEX_OSAL_262        },
    {OSAL_MUTEX_OSAL_263        },
    {OSAL_MUTEX_OSAL_264        },
    {OSAL_MUTEX_OSAL_265        },
    {OSAL_MUTEX_OSAL_266        },
    {OSAL_MUTEX_OSAL_267        },
    {OSAL_MUTEX_OSAL_268        },
    {OSAL_MUTEX_OSAL_269        },
    {OSAL_MUTEX_OSAL_270        },
    {OSAL_MUTEX_OSAL_271        },
    {OSAL_MUTEX_OSAL_272        },
    {OSAL_MUTEX_OSAL_273        },
    {OSAL_MUTEX_OSAL_274        },
    {OSAL_MUTEX_OSAL_275        },
    {OSAL_MUTEX_OSAL_276        },
    {OSAL_MUTEX_OSAL_277        },
    {OSAL_MUTEX_OSAL_278        },
    {OSAL_MUTEX_OSAL_279        },
    {OSAL_MUTEX_OSAL_280        },
    {OSAL_MUTEX_OSAL_281        },
    {OSAL_MUTEX_OSAL_282        },
    {OSAL_MUTEX_OSAL_283        },
    {OSAL_MUTEX_OSAL_284        },
    {OSAL_MUTEX_OSAL_285        },
    {OSAL_MUTEX_OSAL_286        },
    {OSAL_MUTEX_OSAL_287        },
    {OSAL_MUTEX_OSAL_288        },
    {OSAL_MUTEX_OSAL_289        },
    {OSAL_MUTEX_OSAL_290        },
    {OSAL_MUTEX_OSAL_291        },
    {OSAL_MUTEX_OSAL_292        },
    {OSAL_MUTEX_OSAL_293        },
    {OSAL_MUTEX_OSAL_294        },
    {OSAL_MUTEX_OSAL_295        },
    {OSAL_MUTEX_OSAL_296        },
    {OSAL_MUTEX_OSAL_297        },
    {OSAL_MUTEX_OSAL_298        },
    {OSAL_MUTEX_OSAL_299        },
    {OSAL_MUTEX_OSAL_300        },
    {OSAL_MUTEX_OSAL_301        },
    {OSAL_MUTEX_OSAL_302        },
    {OSAL_MUTEX_OSAL_303        },
    {OSAL_MUTEX_OSAL_304        },
    {OSAL_MUTEX_OSAL_305        },
    {OSAL_MUTEX_OSAL_306        },
    {OSAL_MUTEX_OSAL_307        },
    {OSAL_MUTEX_OSAL_308        },
    {OSAL_MUTEX_OSAL_309        },
    {OSAL_MUTEX_OSAL_310        },
    {OSAL_MUTEX_OSAL_311        },
    {OSAL_MUTEX_OSAL_312        },
    {OSAL_MUTEX_OSAL_313        },
    {OSAL_MUTEX_OSAL_314        },
    {OSAL_MUTEX_OSAL_315        },
    {OSAL_MUTEX_OSAL_316        },
    {OSAL_MUTEX_OSAL_317        },
    {OSAL_MUTEX_OSAL_318        },
    {OSAL_MUTEX_OSAL_319        },
    {OSAL_MUTEX_OSAL_320        },
    {OSAL_MUTEX_OSAL_321        },
    {OSAL_MUTEX_OSAL_322        },
    {OSAL_MUTEX_OSAL_323        },
    {OSAL_MUTEX_OSAL_324        },
    {OSAL_MUTEX_OSAL_325        },
    {OSAL_MUTEX_OSAL_326        },
    {OSAL_MUTEX_OSAL_327        },
    {OSAL_MUTEX_OSAL_328        },
    {OSAL_MUTEX_OSAL_329        },
    {OSAL_MUTEX_OSAL_330        },
    {OSAL_MUTEX_OSAL_331        },
    {OSAL_MUTEX_OSAL_332        },
    {OSAL_MUTEX_OSAL_333        },
    {OSAL_MUTEX_OSAL_334        },
    {OSAL_MUTEX_OSAL_335        },
    {OSAL_MUTEX_OSAL_336        },
    {OSAL_MUTEX_OSAL_337        },
    {OSAL_MUTEX_OSAL_338        },
    {OSAL_MUTEX_OSAL_339        },
    {OSAL_MUTEX_OSAL_340        },
    {OSAL_MUTEX_OSAL_341        },
    {OSAL_MUTEX_OSAL_342        },
    {OSAL_MUTEX_OSAL_343        },
    {OSAL_MUTEX_OSAL_344        },
    {OSAL_MUTEX_OSAL_345        },
    {OSAL_MUTEX_OSAL_346        },
    {OSAL_MUTEX_OSAL_347        },
    {OSAL_MUTEX_OSAL_348        },
    {OSAL_MUTEX_OSAL_349        },
    {OSAL_MUTEX_OSAL_350        },
    {OSAL_MUTEX_OSAL_351        },
    {OSAL_MUTEX_OSAL_352        },
    {OSAL_MUTEX_OSAL_353        },
    {OSAL_MUTEX_OSAL_354        },
    {OSAL_MUTEX_OSAL_355        },
    {OSAL_MUTEX_OSAL_356        },
    {OSAL_MUTEX_OSAL_357        },
    {OSAL_MUTEX_OSAL_358        },
    {OSAL_MUTEX_OSAL_359        },
    {OSAL_MUTEX_OSAL_360        },
    {OSAL_MUTEX_OSAL_361        },
    {OSAL_MUTEX_OSAL_362        },
    {OSAL_MUTEX_OSAL_363        },
    {OSAL_MUTEX_OSAL_364        },
    {OSAL_MUTEX_OSAL_365        },
    {OSAL_MUTEX_OSAL_366        },
    {OSAL_MUTEX_OSAL_367        },
    {OSAL_MUTEX_OSAL_368        },
    {OSAL_MUTEX_OSAL_369        },
    {OSAL_MUTEX_OSAL_370        },
    {OSAL_MUTEX_OSAL_371        },
    {OSAL_MUTEX_OSAL_372        },
    {OSAL_MUTEX_OSAL_373        },
    {OSAL_MUTEX_OSAL_374        },
    {OSAL_MUTEX_OSAL_375        },
    {OSAL_MUTEX_OSAL_376        },
    {OSAL_MUTEX_OSAL_377        },
    {OSAL_MUTEX_OSAL_378        },
    {OSAL_MUTEX_OSAL_379        },
    {OSAL_MUTEX_OSAL_380        },
    {OSAL_MUTEX_OSAL_381        },
    {OSAL_MUTEX_OSAL_382        },
    {OSAL_MUTEX_OSAL_383        },
    {OSAL_MUTEX_OSAL_384        },
    {OSAL_MUTEX_OSAL_385        },
    {OSAL_MUTEX_OSAL_386        },
    {OSAL_MUTEX_OSAL_387        },
    {OSAL_MUTEX_OSAL_388        },
    {OSAL_MUTEX_OSAL_389        },
    {OSAL_MUTEX_OSAL_390        },
    {OSAL_MUTEX_OSAL_391        },
    {OSAL_MUTEX_OSAL_392        },
    {OSAL_MUTEX_OSAL_393        },
    {OSAL_MUTEX_OSAL_394        },
    {OSAL_MUTEX_OSAL_395        },
    {OSAL_MUTEX_OSAL_396        },
    {OSAL_MUTEX_OSAL_397        },
    {OSAL_MUTEX_OSAL_398        },
    {OSAL_MUTEX_OSAL_399        },
    {OSAL_MUTEX_OSAL_400        },
    {OSAL_MUTEX_OSAL_401        },
    {OSAL_MUTEX_OSAL_402        },
    {OSAL_MUTEX_OSAL_403        },
    {OSAL_MUTEX_OSAL_404        },
    {OSAL_MUTEX_OSAL_405        },
    {OSAL_MUTEX_OSAL_406        },
    {OSAL_MUTEX_OSAL_407        },
    {OSAL_MUTEX_OSAL_408        },
    {OSAL_MUTEX_OSAL_409        },
    {OSAL_MUTEX_OSAL_410        },
    {OSAL_MUTEX_OSAL_411        },
    {OSAL_MUTEX_OSAL_412        },
    {OSAL_MUTEX_OSAL_413        },
    {OSAL_MUTEX_OSAL_414        },
    {OSAL_MUTEX_OSAL_415        },
    {OSAL_MUTEX_OSAL_416        },
    {OSAL_MUTEX_OSAL_417        },
    {OSAL_MUTEX_OSAL_418        },
    {OSAL_MUTEX_OSAL_419        },
    {OSAL_MUTEX_OSAL_420        },
    {OSAL_MUTEX_OSAL_421        },
    {OSAL_MUTEX_OSAL_422        },
    {OSAL_MUTEX_OSAL_423        },
    {OSAL_MUTEX_OSAL_424        },
    {OSAL_MUTEX_OSAL_425        },
    {OSAL_MUTEX_OSAL_426        },
    {OSAL_MUTEX_OSAL_427        },
    {OSAL_MUTEX_OSAL_428        },
    {OSAL_MUTEX_OSAL_429        },
    {OSAL_MUTEX_OSAL_430        },
    {OSAL_MUTEX_OSAL_431        },
    {OSAL_MUTEX_OSAL_432        },
    {OSAL_MUTEX_OSAL_433        },
    {OSAL_MUTEX_OSAL_434        },
    {OSAL_MUTEX_OSAL_435        },
    {OSAL_MUTEX_OSAL_436        },
    {OSAL_MUTEX_OSAL_437        },
    {OSAL_MUTEX_OSAL_438        },
    {OSAL_MUTEX_OSAL_439        },
    {OSAL_MUTEX_OSAL_440        },
    {OSAL_MUTEX_OSAL_441        },
    {OSAL_MUTEX_OSAL_442        },
    {OSAL_MUTEX_OSAL_443        },
    {OSAL_MUTEX_OSAL_444        },
    {OSAL_MUTEX_OSAL_445        },
    {OSAL_MUTEX_OSAL_446        },
    {OSAL_MUTEX_OSAL_447        },
    {OSAL_MUTEX_OSAL_448        },
    {OSAL_MUTEX_OSAL_449        },
    {OSAL_MUTEX_OSAL_450        },
    {OSAL_MUTEX_OSAL_451        },
    {OSAL_MUTEX_OSAL_452        },
    {OSAL_MUTEX_OSAL_453        },
    {OSAL_MUTEX_OSAL_454        },
    {OSAL_MUTEX_OSAL_455        },
    {OSAL_MUTEX_OSAL_456        },
    {OSAL_MUTEX_OSAL_457        },
    {OSAL_MUTEX_OSAL_458        },
    {OSAL_MUTEX_OSAL_459        },
    {OSAL_MUTEX_OSAL_460        },
    {OSAL_MUTEX_OSAL_461        },
    {OSAL_MUTEX_OSAL_462        },
    {OSAL_MUTEX_OSAL_463        },
    {OSAL_MUTEX_OSAL_464        },
    {OSAL_MUTEX_OSAL_465        },
    {OSAL_MUTEX_OSAL_466        },
    {OSAL_MUTEX_OSAL_467        },
    {OSAL_MUTEX_OSAL_468        },
    {OSAL_MUTEX_OSAL_469        },
    {OSAL_MUTEX_OSAL_470        },
    {OSAL_MUTEX_OSAL_471        },
    {OSAL_MUTEX_OSAL_472        },
    {OSAL_MUTEX_OSAL_473        },
    {OSAL_MUTEX_OSAL_474        },
    {OSAL_MUTEX_OSAL_475        },
    {OSAL_MUTEX_OSAL_476        },
    {OSAL_MUTEX_OSAL_477        },
    {OSAL_MUTEX_OSAL_478        },
    {OSAL_MUTEX_OSAL_479        },
    {OSAL_MUTEX_OSAL_480        },
    {OSAL_MUTEX_OSAL_481        },
    {OSAL_MUTEX_OSAL_482        },
    {OSAL_MUTEX_OSAL_483        },
    {OSAL_MUTEX_OSAL_484        },
    {OSAL_MUTEX_OSAL_485        },
    {OSAL_MUTEX_OSAL_486        },
    {OSAL_MUTEX_OSAL_487        },
    {OSAL_MUTEX_OSAL_488        },
    {OSAL_MUTEX_OSAL_489        },
    {OSAL_MUTEX_OSAL_490        },
    {OSAL_MUTEX_OSAL_491        },
    {OSAL_MUTEX_OSAL_492        },
    {OSAL_MUTEX_OSAL_493        },
    {OSAL_MUTEX_OSAL_494        },
    {OSAL_MUTEX_OSAL_495        },
    {OSAL_MUTEX_OSAL_496        },
    {OSAL_MUTEX_OSAL_497        },
    {OSAL_MUTEX_OSAL_498        },
    {OSAL_MUTEX_OSAL_499        },
    {OSAL_MUTEX_OSAL_500        },
#endif
    /* tentative. internal reserve */
    {OSAL_MUTEX_OSAL_PMA       },

    {.id = OSAL_MUTEX_INVALID_ID}
};
#endif

/*******************************************************************************************************************//**
 * @var g_osal_cond_config
 * osal user configuration : cond configuration table
***********************************************************************************************************************/
#ifndef TEST_LINUX
const st_osal_rcfg_cond_config_t g_osal_cond_config[] =
{
    /* cond id               */
    /* xOS2 resource information */
    {OSAL_COND_OSAL_00      },
    {OSAL_COND_OSAL_01      },
    {OSAL_COND_OSAL_02      },
    {OSAL_COND_OSAL_03      },
    {OSAL_COND_OSAL_04      },
    {OSAL_COND_OSAL_05      },
    {OSAL_COND_OSAL_06      },
    {OSAL_COND_OSAL_07      },
    {OSAL_COND_OSAL_08      },
    {OSAL_COND_OSAL_09      },
    {OSAL_COND_OSAL_10      },
#if defined(ENABLE_LONGTIME_TEST)
    {OSAL_COND_OSAL_11      },
    {OSAL_COND_OSAL_12      },
    {OSAL_COND_OSAL_13      },
    {OSAL_COND_OSAL_14      },
    {OSAL_COND_OSAL_15      },
    {OSAL_COND_OSAL_16      },
    {OSAL_COND_OSAL_17      },
    {OSAL_COND_OSAL_18      },
    {OSAL_COND_OSAL_19      },
    {OSAL_COND_OSAL_20      },
    {OSAL_COND_OSAL_21      },
    {OSAL_COND_OSAL_22      },
    {OSAL_COND_OSAL_23      },
    {OSAL_COND_OSAL_24      },
    {OSAL_COND_OSAL_25      },
    {OSAL_COND_OSAL_26      },
    {OSAL_COND_OSAL_27      },
    {OSAL_COND_OSAL_28      },
    {OSAL_COND_OSAL_29      },
    {OSAL_COND_OSAL_30      },
    {OSAL_COND_OSAL_31      },
    {OSAL_COND_OSAL_32      },
    {OSAL_COND_OSAL_33      },
    {OSAL_COND_OSAL_34      },
    {OSAL_COND_OSAL_35      },
    {OSAL_COND_OSAL_36      },
    {OSAL_COND_OSAL_37      },
    {OSAL_COND_OSAL_38      },
    {OSAL_COND_OSAL_39      },
    {OSAL_COND_OSAL_40      },
    {OSAL_COND_OSAL_41      },
    {OSAL_COND_OSAL_42      },
    {OSAL_COND_OSAL_43      },
    {OSAL_COND_OSAL_44      },
    {OSAL_COND_OSAL_45      },
    {OSAL_COND_OSAL_46      },
    {OSAL_COND_OSAL_47      },
    {OSAL_COND_OSAL_48      },
    {OSAL_COND_OSAL_49      },
    {OSAL_COND_OSAL_50      },
    {OSAL_COND_OSAL_51      },
    {OSAL_COND_OSAL_52      },
    {OSAL_COND_OSAL_53      },
    {OSAL_COND_OSAL_54      },
    {OSAL_COND_OSAL_55      },
    {OSAL_COND_OSAL_56      },
    {OSAL_COND_OSAL_57      },
    {OSAL_COND_OSAL_58      },
    {OSAL_COND_OSAL_59      },
    {OSAL_COND_OSAL_60      },
    {OSAL_COND_OSAL_61      },
    {OSAL_COND_OSAL_62      },
    {OSAL_COND_OSAL_63      },
    {OSAL_COND_OSAL_64      },
    {OSAL_COND_OSAL_65      },
    {OSAL_COND_OSAL_66      },
    {OSAL_COND_OSAL_67      },
    {OSAL_COND_OSAL_68      },
    {OSAL_COND_OSAL_69      },
    {OSAL_COND_OSAL_70      },
    {OSAL_COND_OSAL_71      },
    {OSAL_COND_OSAL_72      },
    {OSAL_COND_OSAL_73      },
    {OSAL_COND_OSAL_74      },
    {OSAL_COND_OSAL_75      },
    {OSAL_COND_OSAL_76      },
    {OSAL_COND_OSAL_77      },
    {OSAL_COND_OSAL_78      },
    {OSAL_COND_OSAL_79      },
    {OSAL_COND_OSAL_80      },
    {OSAL_COND_OSAL_81      },
    {OSAL_COND_OSAL_82      },
    {OSAL_COND_OSAL_83      },
    {OSAL_COND_OSAL_84      },
    {OSAL_COND_OSAL_85      },
    {OSAL_COND_OSAL_86      },
    {OSAL_COND_OSAL_87      },
    {OSAL_COND_OSAL_88      },
    {OSAL_COND_OSAL_89      },
    {OSAL_COND_OSAL_90      },
    {OSAL_COND_OSAL_91      },
    {OSAL_COND_OSAL_92      },
    {OSAL_COND_OSAL_93      },
    {OSAL_COND_OSAL_94      },
    {OSAL_COND_OSAL_95      },
    {OSAL_COND_OSAL_96      },
    {OSAL_COND_OSAL_97      },
    {OSAL_COND_OSAL_98      },
    {OSAL_COND_OSAL_99      },
    {OSAL_COND_OSAL_100      },
    {OSAL_COND_OSAL_101      },
    {OSAL_COND_OSAL_102      },
    {OSAL_COND_OSAL_103      },
    {OSAL_COND_OSAL_104      },
    {OSAL_COND_OSAL_105      },
    {OSAL_COND_OSAL_106      },
    {OSAL_COND_OSAL_107      },
    {OSAL_COND_OSAL_108      },
    {OSAL_COND_OSAL_109      },
    {OSAL_COND_OSAL_110      },
    {OSAL_COND_OSAL_111      },
    {OSAL_COND_OSAL_112      },
    {OSAL_COND_OSAL_113      },
    {OSAL_COND_OSAL_114      },
    {OSAL_COND_OSAL_115      },
    {OSAL_COND_OSAL_116      },
    {OSAL_COND_OSAL_117      },
    {OSAL_COND_OSAL_118      },
    {OSAL_COND_OSAL_119      },
    {OSAL_COND_OSAL_120      },
    {OSAL_COND_OSAL_121      },
    {OSAL_COND_OSAL_122      },
    {OSAL_COND_OSAL_123      },
    {OSAL_COND_OSAL_124      },
    {OSAL_COND_OSAL_125      },
    {OSAL_COND_OSAL_126      },
    {OSAL_COND_OSAL_127      },
    {OSAL_COND_OSAL_128      },
    {OSAL_COND_OSAL_129      },
    {OSAL_COND_OSAL_130      },
    {OSAL_COND_OSAL_131      },
    {OSAL_COND_OSAL_132      },
    {OSAL_COND_OSAL_133      },
    {OSAL_COND_OSAL_134      },
    {OSAL_COND_OSAL_135      },
    {OSAL_COND_OSAL_136      },
    {OSAL_COND_OSAL_137      },
    {OSAL_COND_OSAL_138      },
    {OSAL_COND_OSAL_139      },
    {OSAL_COND_OSAL_140      },
    {OSAL_COND_OSAL_141      },
    {OSAL_COND_OSAL_142      },
    {OSAL_COND_OSAL_143      },
    {OSAL_COND_OSAL_144      },
    {OSAL_COND_OSAL_145      },
    {OSAL_COND_OSAL_146      },
    {OSAL_COND_OSAL_147      },
    {OSAL_COND_OSAL_148      },
    {OSAL_COND_OSAL_149      },
    {OSAL_COND_OSAL_150      },
    {OSAL_COND_OSAL_151      },
    {OSAL_COND_OSAL_152      },
    {OSAL_COND_OSAL_153      },
    {OSAL_COND_OSAL_154      },
    {OSAL_COND_OSAL_155      },
    {OSAL_COND_OSAL_156      },
    {OSAL_COND_OSAL_157      },
    {OSAL_COND_OSAL_158      },
    {OSAL_COND_OSAL_159      },
    {OSAL_COND_OSAL_160      },
    {OSAL_COND_OSAL_161      },
    {OSAL_COND_OSAL_162      },
    {OSAL_COND_OSAL_163      },
    {OSAL_COND_OSAL_164      },
    {OSAL_COND_OSAL_165      },
    {OSAL_COND_OSAL_166      },
    {OSAL_COND_OSAL_167      },
    {OSAL_COND_OSAL_168      },
    {OSAL_COND_OSAL_169      },
    {OSAL_COND_OSAL_170      },
    {OSAL_COND_OSAL_171      },
    {OSAL_COND_OSAL_172      },
    {OSAL_COND_OSAL_173      },
    {OSAL_COND_OSAL_174      },
    {OSAL_COND_OSAL_175      },
    {OSAL_COND_OSAL_176      },
    {OSAL_COND_OSAL_177      },
    {OSAL_COND_OSAL_178      },
    {OSAL_COND_OSAL_179      },
    {OSAL_COND_OSAL_180      },
    {OSAL_COND_OSAL_181      },
    {OSAL_COND_OSAL_182      },
    {OSAL_COND_OSAL_183      },
    {OSAL_COND_OSAL_184      },
    {OSAL_COND_OSAL_185      },
    {OSAL_COND_OSAL_186      },
    {OSAL_COND_OSAL_187      },
    {OSAL_COND_OSAL_188      },
    {OSAL_COND_OSAL_189      },
    {OSAL_COND_OSAL_190      },
    {OSAL_COND_OSAL_191      },
    {OSAL_COND_OSAL_192      },
    {OSAL_COND_OSAL_193      },
    {OSAL_COND_OSAL_194      },
    {OSAL_COND_OSAL_195      },
    {OSAL_COND_OSAL_196      },
    {OSAL_COND_OSAL_197      },
    {OSAL_COND_OSAL_198      },
    {OSAL_COND_OSAL_199      },
    {OSAL_COND_OSAL_200      },
    {OSAL_COND_OSAL_201      },
    {OSAL_COND_OSAL_202      },
    {OSAL_COND_OSAL_203      },
    {OSAL_COND_OSAL_204      },
    {OSAL_COND_OSAL_205      },
    {OSAL_COND_OSAL_206      },
    {OSAL_COND_OSAL_207      },
    {OSAL_COND_OSAL_208      },
    {OSAL_COND_OSAL_209      },
    {OSAL_COND_OSAL_210      },
    {OSAL_COND_OSAL_211      },
    {OSAL_COND_OSAL_212      },
    {OSAL_COND_OSAL_213      },
    {OSAL_COND_OSAL_214      },
    {OSAL_COND_OSAL_215      },
    {OSAL_COND_OSAL_216      },
    {OSAL_COND_OSAL_217      },
    {OSAL_COND_OSAL_218      },
    {OSAL_COND_OSAL_219      },
    {OSAL_COND_OSAL_220      },
    {OSAL_COND_OSAL_221      },
    {OSAL_COND_OSAL_222      },
    {OSAL_COND_OSAL_223      },
    {OSAL_COND_OSAL_224      },
    {OSAL_COND_OSAL_225      },
    {OSAL_COND_OSAL_226      },
    {OSAL_COND_OSAL_227      },
    {OSAL_COND_OSAL_228      },
    {OSAL_COND_OSAL_229      },
    {OSAL_COND_OSAL_230      },
    {OSAL_COND_OSAL_231      },
    {OSAL_COND_OSAL_232      },
    {OSAL_COND_OSAL_233      },
    {OSAL_COND_OSAL_234      },
    {OSAL_COND_OSAL_235      },
    {OSAL_COND_OSAL_236      },
    {OSAL_COND_OSAL_237      },
    {OSAL_COND_OSAL_238      },
    {OSAL_COND_OSAL_239      },
    {OSAL_COND_OSAL_240      },
    {OSAL_COND_OSAL_241      },
    {OSAL_COND_OSAL_242      },
    {OSAL_COND_OSAL_243      },
    {OSAL_COND_OSAL_244      },
    {OSAL_COND_OSAL_245      },
    {OSAL_COND_OSAL_246      },
    {OSAL_COND_OSAL_247      },
    {OSAL_COND_OSAL_248      },
    {OSAL_COND_OSAL_249      },
    {OSAL_COND_OSAL_250      },
    {OSAL_COND_OSAL_251      },
    {OSAL_COND_OSAL_252      },
    {OSAL_COND_OSAL_253      },
    {OSAL_COND_OSAL_254      },
    {OSAL_COND_OSAL_255      },
    {OSAL_COND_OSAL_256      },
    {OSAL_COND_OSAL_257      },
    {OSAL_COND_OSAL_258      },
    {OSAL_COND_OSAL_259      },
    {OSAL_COND_OSAL_260      },
    {OSAL_COND_OSAL_261      },
    {OSAL_COND_OSAL_262      },
    {OSAL_COND_OSAL_263      },
    {OSAL_COND_OSAL_264      },
    {OSAL_COND_OSAL_265      },
    {OSAL_COND_OSAL_266      },
    {OSAL_COND_OSAL_267      },
    {OSAL_COND_OSAL_268      },
    {OSAL_COND_OSAL_269      },
    {OSAL_COND_OSAL_270      },
    {OSAL_COND_OSAL_271      },
    {OSAL_COND_OSAL_272      },
    {OSAL_COND_OSAL_273      },
    {OSAL_COND_OSAL_274      },
    {OSAL_COND_OSAL_275      },
    {OSAL_COND_OSAL_276      },
    {OSAL_COND_OSAL_277      },
    {OSAL_COND_OSAL_278      },
    {OSAL_COND_OSAL_279      },
    {OSAL_COND_OSAL_280      },
    {OSAL_COND_OSAL_281      },
    {OSAL_COND_OSAL_282      },
    {OSAL_COND_OSAL_283      },
    {OSAL_COND_OSAL_284      },
    {OSAL_COND_OSAL_285      },
    {OSAL_COND_OSAL_286      },
    {OSAL_COND_OSAL_287      },
    {OSAL_COND_OSAL_288      },
    {OSAL_COND_OSAL_289      },
    {OSAL_COND_OSAL_290      },
    {OSAL_COND_OSAL_291      },
    {OSAL_COND_OSAL_292      },
    {OSAL_COND_OSAL_293      },
    {OSAL_COND_OSAL_294      },
    {OSAL_COND_OSAL_295      },
    {OSAL_COND_OSAL_296      },
    {OSAL_COND_OSAL_297      },
    {OSAL_COND_OSAL_298      },
    {OSAL_COND_OSAL_299      },
    {OSAL_COND_OSAL_300      },
    {OSAL_COND_OSAL_301      },
    {OSAL_COND_OSAL_302      },
    {OSAL_COND_OSAL_303      },
    {OSAL_COND_OSAL_304      },
    {OSAL_COND_OSAL_305      },
    {OSAL_COND_OSAL_306      },
    {OSAL_COND_OSAL_307      },
    {OSAL_COND_OSAL_308      },
    {OSAL_COND_OSAL_309      },
    {OSAL_COND_OSAL_310      },
    {OSAL_COND_OSAL_311      },
    {OSAL_COND_OSAL_312      },
    {OSAL_COND_OSAL_313      },
    {OSAL_COND_OSAL_314      },
    {OSAL_COND_OSAL_315      },
    {OSAL_COND_OSAL_316      },
    {OSAL_COND_OSAL_317      },
    {OSAL_COND_OSAL_318      },
    {OSAL_COND_OSAL_319      },
    {OSAL_COND_OSAL_320      },
    {OSAL_COND_OSAL_321      },
    {OSAL_COND_OSAL_322      },
    {OSAL_COND_OSAL_323      },
    {OSAL_COND_OSAL_324      },
    {OSAL_COND_OSAL_325      },
    {OSAL_COND_OSAL_326      },
    {OSAL_COND_OSAL_327      },
    {OSAL_COND_OSAL_328      },
    {OSAL_COND_OSAL_329      },
    {OSAL_COND_OSAL_330      },
    {OSAL_COND_OSAL_331      },
    {OSAL_COND_OSAL_332      },
    {OSAL_COND_OSAL_333      },
    {OSAL_COND_OSAL_334      },
    {OSAL_COND_OSAL_335      },
    {OSAL_COND_OSAL_336      },
    {OSAL_COND_OSAL_337      },
    {OSAL_COND_OSAL_338      },
    {OSAL_COND_OSAL_339      },
    {OSAL_COND_OSAL_340      },
    {OSAL_COND_OSAL_341      },
    {OSAL_COND_OSAL_342      },
    {OSAL_COND_OSAL_343      },
    {OSAL_COND_OSAL_344      },
    {OSAL_COND_OSAL_345      },
    {OSAL_COND_OSAL_346      },
    {OSAL_COND_OSAL_347      },
    {OSAL_COND_OSAL_348      },
    {OSAL_COND_OSAL_349      },
    {OSAL_COND_OSAL_350      },
    {OSAL_COND_OSAL_351      },
    {OSAL_COND_OSAL_352      },
    {OSAL_COND_OSAL_353      },
    {OSAL_COND_OSAL_354      },
    {OSAL_COND_OSAL_355      },
    {OSAL_COND_OSAL_356      },
    {OSAL_COND_OSAL_357      },
    {OSAL_COND_OSAL_358      },
    {OSAL_COND_OSAL_359      },
    {OSAL_COND_OSAL_360      },
    {OSAL_COND_OSAL_361      },
    {OSAL_COND_OSAL_362      },
    {OSAL_COND_OSAL_363      },
    {OSAL_COND_OSAL_364      },
    {OSAL_COND_OSAL_365      },
    {OSAL_COND_OSAL_366      },
    {OSAL_COND_OSAL_367      },
    {OSAL_COND_OSAL_368      },
    {OSAL_COND_OSAL_369      },
    {OSAL_COND_OSAL_370      },
    {OSAL_COND_OSAL_371      },
    {OSAL_COND_OSAL_372      },
    {OSAL_COND_OSAL_373      },
    {OSAL_COND_OSAL_374      },
    {OSAL_COND_OSAL_375      },
    {OSAL_COND_OSAL_376      },
    {OSAL_COND_OSAL_377      },
    {OSAL_COND_OSAL_378      },
    {OSAL_COND_OSAL_379      },
    {OSAL_COND_OSAL_380      },
    {OSAL_COND_OSAL_381      },
    {OSAL_COND_OSAL_382      },
    {OSAL_COND_OSAL_383      },
    {OSAL_COND_OSAL_384      },
    {OSAL_COND_OSAL_385      },
    {OSAL_COND_OSAL_386      },
    {OSAL_COND_OSAL_387      },
    {OSAL_COND_OSAL_388      },
    {OSAL_COND_OSAL_389      },
    {OSAL_COND_OSAL_390      },
    {OSAL_COND_OSAL_391      },
    {OSAL_COND_OSAL_392      },
    {OSAL_COND_OSAL_393      },
    {OSAL_COND_OSAL_394      },
    {OSAL_COND_OSAL_395      },
    {OSAL_COND_OSAL_396      },
    {OSAL_COND_OSAL_397      },
    {OSAL_COND_OSAL_398      },
    {OSAL_COND_OSAL_399      },
    {OSAL_COND_OSAL_400      },
    {OSAL_COND_OSAL_401      },
    {OSAL_COND_OSAL_402      },
    {OSAL_COND_OSAL_403      },
    {OSAL_COND_OSAL_404      },
    {OSAL_COND_OSAL_405      },
    {OSAL_COND_OSAL_406      },
    {OSAL_COND_OSAL_407      },
    {OSAL_COND_OSAL_408      },
    {OSAL_COND_OSAL_409      },
    {OSAL_COND_OSAL_410      },
    {OSAL_COND_OSAL_411      },
    {OSAL_COND_OSAL_412      },
    {OSAL_COND_OSAL_413      },
    {OSAL_COND_OSAL_414      },
    {OSAL_COND_OSAL_415      },
    {OSAL_COND_OSAL_416      },
    {OSAL_COND_OSAL_417      },
    {OSAL_COND_OSAL_418      },
    {OSAL_COND_OSAL_419      },
    {OSAL_COND_OSAL_420      },
    {OSAL_COND_OSAL_421      },
    {OSAL_COND_OSAL_422      },
    {OSAL_COND_OSAL_423      },
    {OSAL_COND_OSAL_424      },
    {OSAL_COND_OSAL_425      },
    {OSAL_COND_OSAL_426      },
    {OSAL_COND_OSAL_427      },
    {OSAL_COND_OSAL_428      },
    {OSAL_COND_OSAL_429      },
    {OSAL_COND_OSAL_430      },
    {OSAL_COND_OSAL_431      },
    {OSAL_COND_OSAL_432      },
    {OSAL_COND_OSAL_433      },
    {OSAL_COND_OSAL_434      },
    {OSAL_COND_OSAL_435      },
    {OSAL_COND_OSAL_436      },
    {OSAL_COND_OSAL_437      },
    {OSAL_COND_OSAL_438      },
    {OSAL_COND_OSAL_439      },
    {OSAL_COND_OSAL_440      },
    {OSAL_COND_OSAL_441      },
    {OSAL_COND_OSAL_442      },
    {OSAL_COND_OSAL_443      },
    {OSAL_COND_OSAL_444      },
    {OSAL_COND_OSAL_445      },
    {OSAL_COND_OSAL_446      },
    {OSAL_COND_OSAL_447      },
    {OSAL_COND_OSAL_448      },
    {OSAL_COND_OSAL_449      },
    {OSAL_COND_OSAL_450      },
    {OSAL_COND_OSAL_451      },
    {OSAL_COND_OSAL_452      },
    {OSAL_COND_OSAL_453      },
    {OSAL_COND_OSAL_454      },
    {OSAL_COND_OSAL_455      },
    {OSAL_COND_OSAL_456      },
    {OSAL_COND_OSAL_457      },
    {OSAL_COND_OSAL_458      },
    {OSAL_COND_OSAL_459      },
    {OSAL_COND_OSAL_460      },
    {OSAL_COND_OSAL_461      },
    {OSAL_COND_OSAL_462      },
    {OSAL_COND_OSAL_463      },
    {OSAL_COND_OSAL_464      },
    {OSAL_COND_OSAL_465      },
    {OSAL_COND_OSAL_466      },
    {OSAL_COND_OSAL_467      },
    {OSAL_COND_OSAL_468      },
    {OSAL_COND_OSAL_469      },
    {OSAL_COND_OSAL_470      },
    {OSAL_COND_OSAL_471      },
    {OSAL_COND_OSAL_472      },
    {OSAL_COND_OSAL_473      },
    {OSAL_COND_OSAL_474      },
    {OSAL_COND_OSAL_475      },
    {OSAL_COND_OSAL_476      },
    {OSAL_COND_OSAL_477      },
    {OSAL_COND_OSAL_478      },
    {OSAL_COND_OSAL_479      },
    {OSAL_COND_OSAL_480      },
    {OSAL_COND_OSAL_481      },
    {OSAL_COND_OSAL_482      },
    {OSAL_COND_OSAL_483      },
    {OSAL_COND_OSAL_484      },
    {OSAL_COND_OSAL_485      },
    {OSAL_COND_OSAL_486      },
    {OSAL_COND_OSAL_487      },
    {OSAL_COND_OSAL_488      },
    {OSAL_COND_OSAL_489      },
    {OSAL_COND_OSAL_490      },
    {OSAL_COND_OSAL_491      },
    {OSAL_COND_OSAL_492      },
    {OSAL_COND_OSAL_493      },
    {OSAL_COND_OSAL_494      },
    {OSAL_COND_OSAL_495      },
    {OSAL_COND_OSAL_496      },
    {OSAL_COND_OSAL_497      },
    {OSAL_COND_OSAL_498      },
    {OSAL_COND_OSAL_499      },
    {OSAL_COND_OSAL_500      },
#endif
    {.id = OSAL_COND_INVALID_ID}
};
#endif

/*******************************************************************************************************************//**
 * @var g_osal_mq_config
 * osal user configuration : message queue configuration table
***********************************************************************************************************************/
#ifndef TEST_LINUX
const st_osal_mq_rcfg_config_t g_osal_mq_config[] =
{
    /* mq id,                       max_num_msg, msg_size */
    {9999U , {2, 4}  },

    /* xOS2 resource information */
    {OSAL_MQ_OSAL_00,                        { 2,  4}  },
    {OSAL_MQ_OSAL_01,                        { 1,  4}  },
    {OSAL_MQ_OSAL_02,                        { 1,  4}  },
    {OSAL_MQ_OSAL_03,                        {10,  4}  },
    {OSAL_MQ_OSAL_04,                        { 1,  4}  },
    {OSAL_MQ_OSAL_05,                        { 1,  4}  },
    {OSAL_MQ_OSAL_06,                        { 1,  4}  },
    {OSAL_MQ_OSAL_07,                        { 1,  4}  },
    {OSAL_MQ_OSAL_08,                        { 1,  4}  },
    {OSAL_MQ_OSAL_09,                        { 1,  4}  },
    {OSAL_MQ_OSAL_10,                        { 1,  4}  },
#if defined(ENABLE_LONGTIME_TEST)
    {OSAL_MQ_OSAL_11,                        { 1,  4}  },
    {OSAL_MQ_OSAL_12,                        { 1,  4}  },
    {OSAL_MQ_OSAL_13,                        { 1,  4}  },
    {OSAL_MQ_OSAL_14,                        { 1,  4}  },
    {OSAL_MQ_OSAL_15,                        { 1,  4}  },
    {OSAL_MQ_OSAL_16,                        { 1,  4}  },
    {OSAL_MQ_OSAL_17,                        { 1,  4}  },
    {OSAL_MQ_OSAL_18,                        { 1,  4}  },
    {OSAL_MQ_OSAL_19,                        { 1,  4}  },
    {OSAL_MQ_OSAL_20,                        { 1,  4}  },
    {OSAL_MQ_OSAL_21,                        {16, 64}  },
    {OSAL_MQ_OSAL_22,                        {16, 64}  },
    {OSAL_MQ_OSAL_23,                        {16, 64}  },
    {OSAL_MQ_OSAL_24,                        {16, 64}  },
    {OSAL_MQ_OSAL_25,                        {16, 64}  },
    {OSAL_MQ_OSAL_26,                        {16, 64}  },
    {OSAL_MQ_OSAL_27,                        {16, 64}  },
    {OSAL_MQ_OSAL_28,                        {16, 64}  },
    {OSAL_MQ_OSAL_29,                        {16, 64}  },
    {OSAL_MQ_OSAL_30,                        {16, 64}  },
    {OSAL_MQ_OSAL_31,                        {16, 64}  },
    {OSAL_MQ_OSAL_32,                        {16, 64}  },
    {OSAL_MQ_OSAL_33,                        {16, 64}  },
    {OSAL_MQ_OSAL_34,                        {16, 64}  },
    {OSAL_MQ_OSAL_35,                        {16, 64}  },
    {OSAL_MQ_OSAL_36,                        {16, 64}  },
    {OSAL_MQ_OSAL_37,                        {16, 64}  },
    {OSAL_MQ_OSAL_38,                        {16, 64}  },
    {OSAL_MQ_OSAL_39,                        {16, 64}  },
    {OSAL_MQ_OSAL_40,                        {16, 64}  },
    {OSAL_MQ_OSAL_41,                        {16, 64}  },
    {OSAL_MQ_OSAL_42,                        {16, 64}  },
    {OSAL_MQ_OSAL_43,                        {16, 64}  },
    {OSAL_MQ_OSAL_44,                        {16, 64}  },
    {OSAL_MQ_OSAL_45,                        {16, 64}  },
    {OSAL_MQ_OSAL_46,                        {16, 64}  },
    {OSAL_MQ_OSAL_47,                        {16, 64}  },
    {OSAL_MQ_OSAL_48,                        {16, 64}  },
    {OSAL_MQ_OSAL_49,                        {16, 64}  },
    {OSAL_MQ_OSAL_50,                        {16, 64}  },
    {OSAL_MQ_OSAL_51,                        {16, 64}  },
    {OSAL_MQ_OSAL_52,                        {16, 64}  },
    {OSAL_MQ_OSAL_53,                        {16, 64}  },
    {OSAL_MQ_OSAL_54,                        {16, 64}  },
    {OSAL_MQ_OSAL_55,                        {16, 64}  },
    {OSAL_MQ_OSAL_56,                        {16, 64}  },
    {OSAL_MQ_OSAL_57,                        {16, 64}  },
    {OSAL_MQ_OSAL_58,                        {16, 64}  },
    {OSAL_MQ_OSAL_59,                        {16, 64}  },
    {OSAL_MQ_OSAL_60,                        {16, 64}  },
    {OSAL_MQ_OSAL_61,                        {16, 64}  },
    {OSAL_MQ_OSAL_62,                        {16, 64}  },
    {OSAL_MQ_OSAL_63,                        {16, 64}  },
    {OSAL_MQ_OSAL_64,                        {16, 64}  },
    {OSAL_MQ_OSAL_65,                        {16, 64}  },
    {OSAL_MQ_OSAL_66,                        {16, 64}  },
    {OSAL_MQ_OSAL_67,                        {16, 64}  },
    {OSAL_MQ_OSAL_68,                        {16, 64}  },
    {OSAL_MQ_OSAL_69,                        {16, 64}  },
    {OSAL_MQ_OSAL_70,                        {16, 64}  },
    {OSAL_MQ_OSAL_71,                        {16, 64}  },
    {OSAL_MQ_OSAL_72,                        {16, 64}  },
    {OSAL_MQ_OSAL_73,                        {16, 64}  },
    {OSAL_MQ_OSAL_74,                        {16, 64}  },
    {OSAL_MQ_OSAL_75,                        {16, 64}  },
    {OSAL_MQ_OSAL_76,                        {16, 64}  },
    {OSAL_MQ_OSAL_77,                        {16, 64}  },
    {OSAL_MQ_OSAL_78,                        {16, 64}  },
    {OSAL_MQ_OSAL_79,                        {16, 64}  },
    {OSAL_MQ_OSAL_80,                        {16, 64}  },
    {OSAL_MQ_OSAL_81,                        {16, 64}  },
    {OSAL_MQ_OSAL_82,                        {16, 64}  },
    {OSAL_MQ_OSAL_83,                        {16, 64}  },
    {OSAL_MQ_OSAL_84,                        {16, 64}  },
    {OSAL_MQ_OSAL_85,                        {16, 64}  },
    {OSAL_MQ_OSAL_86,                        {16, 64}  },
    {OSAL_MQ_OSAL_87,                        {16, 64}  },
    {OSAL_MQ_OSAL_88,                        {16, 64}  },
    {OSAL_MQ_OSAL_89,                        {16, 64}  },
    {OSAL_MQ_OSAL_90,                        {16, 64}  },
    {OSAL_MQ_OSAL_91,                        {16, 64}  },
    {OSAL_MQ_OSAL_92,                        {16, 64}  },
    {OSAL_MQ_OSAL_93,                        {16, 64}  },
    {OSAL_MQ_OSAL_94,                        {16, 64}  },
    {OSAL_MQ_OSAL_95,                        {16, 64}  },
    {OSAL_MQ_OSAL_96,                        {16, 64}  },
    {OSAL_MQ_OSAL_97,                        {16, 64}  },
    {OSAL_MQ_OSAL_98,                        {16, 64}  },
    {OSAL_MQ_OSAL_99,                        {16, 64}  },
    {OSAL_MQ_OSAL_100,                       {16, 64}  },
    {OSAL_MQ_OSAL_101,                       {16, 64}  },
    {OSAL_MQ_OSAL_102,                       {16, 64}  },
    {OSAL_MQ_OSAL_103,                       {16, 64}  },
    {OSAL_MQ_OSAL_104,                       {16, 64}  },
    {OSAL_MQ_OSAL_105,                       {16, 64}  },
    {OSAL_MQ_OSAL_106,                       {16, 64}  },
    {OSAL_MQ_OSAL_107,                       {16, 64}  },
    {OSAL_MQ_OSAL_108,                       {16, 64}  },
    {OSAL_MQ_OSAL_109,                       {16, 64}  },
    {OSAL_MQ_OSAL_110,                       {16, 64}  },
    {OSAL_MQ_OSAL_111,                       {16, 64}  },
    {OSAL_MQ_OSAL_112,                       {16, 64}  },
    {OSAL_MQ_OSAL_113,                       {16, 64}  },
    {OSAL_MQ_OSAL_114,                       {16, 64}  },
    {OSAL_MQ_OSAL_115,                       {16, 64}  },
    {OSAL_MQ_OSAL_116,                       {16, 64}  },
    {OSAL_MQ_OSAL_117,                       {16, 64}  },
    {OSAL_MQ_OSAL_118,                       {16, 64}  },
    {OSAL_MQ_OSAL_119,                       {16, 64}  },
    {OSAL_MQ_OSAL_120,                       {16, 64}  },
    {OSAL_MQ_OSAL_121,                       {16, 64}  },
    {OSAL_MQ_OSAL_122,                       {16, 64}  },
    {OSAL_MQ_OSAL_123,                       {16, 64}  },
    {OSAL_MQ_OSAL_124,                       {16, 64}  },
    {OSAL_MQ_OSAL_125,                       {16, 64}  },
    {OSAL_MQ_OSAL_126,                       {16, 64}  },
    {OSAL_MQ_OSAL_127,                       {16, 64}  },
    {OSAL_MQ_OSAL_128,                       {16, 64}  },
    {OSAL_MQ_OSAL_129,                       {16, 64}  },
    {OSAL_MQ_OSAL_130,                       {16, 64}  },
    {OSAL_MQ_OSAL_131,                       {16, 64}  },
    {OSAL_MQ_OSAL_132,                       {16, 64}  },
    {OSAL_MQ_OSAL_133,                       {16, 64}  },
    {OSAL_MQ_OSAL_134,                       {16, 64}  },
    {OSAL_MQ_OSAL_135,                       {16, 64}  },
    {OSAL_MQ_OSAL_136,                       {16, 64}  },
    {OSAL_MQ_OSAL_137,                       {16, 64}  },
    {OSAL_MQ_OSAL_138,                       {16, 64}  },
    {OSAL_MQ_OSAL_139,                       {16, 64}  },
    {OSAL_MQ_OSAL_140,                       {16, 64}  },
    {OSAL_MQ_OSAL_141,                       {16, 64}  },
    {OSAL_MQ_OSAL_142,                       {16, 64}  },
    {OSAL_MQ_OSAL_143,                       {16, 64}  },
    {OSAL_MQ_OSAL_144,                       {16, 64}  },
    {OSAL_MQ_OSAL_145,                       {16, 64}  },
    {OSAL_MQ_OSAL_146,                       {16, 64}  },
    {OSAL_MQ_OSAL_147,                       {16, 64}  },
    {OSAL_MQ_OSAL_148,                       {16, 64}  },
    {OSAL_MQ_OSAL_149,                       {16, 64}  },
    {OSAL_MQ_OSAL_150,                       {16, 64}  },
    {OSAL_MQ_OSAL_151,                       {16, 64}  },
    {OSAL_MQ_OSAL_152,                       {16, 64}  },
    {OSAL_MQ_OSAL_153,                       {16, 64}  },
    {OSAL_MQ_OSAL_154,                       {16, 64}  },
    {OSAL_MQ_OSAL_155,                       {16, 64}  },
    {OSAL_MQ_OSAL_156,                       {16, 64}  },
    {OSAL_MQ_OSAL_157,                       {16, 64}  },
    {OSAL_MQ_OSAL_158,                       {16, 64}  },
    {OSAL_MQ_OSAL_159,                       {16, 64}  },
    {OSAL_MQ_OSAL_160,                       {16, 64}  },
    {OSAL_MQ_OSAL_161,                       {16, 64}  },
    {OSAL_MQ_OSAL_162,                       {16, 64}  },
    {OSAL_MQ_OSAL_163,                       {16, 64}  },
    {OSAL_MQ_OSAL_164,                       {16, 64}  },
    {OSAL_MQ_OSAL_165,                       {16, 64}  },
    {OSAL_MQ_OSAL_166,                       {16, 64}  },
    {OSAL_MQ_OSAL_167,                       {16, 64}  },
    {OSAL_MQ_OSAL_168,                       {16, 64}  },
    {OSAL_MQ_OSAL_169,                       {16, 64}  },
    {OSAL_MQ_OSAL_170,                       {16, 64}  },
    {OSAL_MQ_OSAL_171,                       {16, 64}  },
    {OSAL_MQ_OSAL_172,                       {16, 64}  },
    {OSAL_MQ_OSAL_173,                       {16, 64}  },
    {OSAL_MQ_OSAL_174,                       {16, 64}  },
    {OSAL_MQ_OSAL_175,                       {16, 64}  },
    {OSAL_MQ_OSAL_176,                       {16, 64}  },
    {OSAL_MQ_OSAL_177,                       {16, 64}  },
    {OSAL_MQ_OSAL_178,                       {16, 64}  },
    {OSAL_MQ_OSAL_179,                       {16, 64}  },
    {OSAL_MQ_OSAL_180,                       {16, 64}  },
    {OSAL_MQ_OSAL_181,                       {16, 64}  },
    {OSAL_MQ_OSAL_182,                       {16, 64}  },
    {OSAL_MQ_OSAL_183,                       {16, 64}  },
    {OSAL_MQ_OSAL_184,                       {16, 64}  },
    {OSAL_MQ_OSAL_185,                       {16, 64}  },
    {OSAL_MQ_OSAL_186,                       {16, 64}  },
    {OSAL_MQ_OSAL_187,                       {16, 64}  },
    {OSAL_MQ_OSAL_188,                       {16, 64}  },
    {OSAL_MQ_OSAL_189,                       {16, 64}  },
    {OSAL_MQ_OSAL_190,                       {16, 64}  },
    {OSAL_MQ_OSAL_191,                       {16, 64}  },
    {OSAL_MQ_OSAL_192,                       {16, 64}  },
    {OSAL_MQ_OSAL_193,                       {16, 64}  },
    {OSAL_MQ_OSAL_194,                       {16, 64}  },
    {OSAL_MQ_OSAL_195,                       {16, 64}  },
    {OSAL_MQ_OSAL_196,                       {16, 64}  },
    {OSAL_MQ_OSAL_197,                       {16, 64}  },
    {OSAL_MQ_OSAL_198,                       {16, 64}  },
    {OSAL_MQ_OSAL_199,                       {16, 64}  },
    {OSAL_MQ_OSAL_200,                       {16, 64}  },
    {OSAL_MQ_OSAL_201,                       {16, 64}  },
    {OSAL_MQ_OSAL_202,                       {16, 64}  },
    {OSAL_MQ_OSAL_203,                       {16, 64}  },
    {OSAL_MQ_OSAL_204,                       {16, 64}  },
    {OSAL_MQ_OSAL_205,                       {16, 64}  },
    {OSAL_MQ_OSAL_206,                       {16, 64}  },
    {OSAL_MQ_OSAL_207,                       {16, 64}  },
    {OSAL_MQ_OSAL_208,                       {16, 64}  },
    {OSAL_MQ_OSAL_209,                       {16, 64}  },
    {OSAL_MQ_OSAL_210,                       {16, 64}  },
    {OSAL_MQ_OSAL_211,                       {16, 64}  },
    {OSAL_MQ_OSAL_212,                       {16, 64}  },
    {OSAL_MQ_OSAL_213,                       {16, 64}  },
    {OSAL_MQ_OSAL_214,                       {16, 64}  },
    {OSAL_MQ_OSAL_215,                       {16, 64}  },
    {OSAL_MQ_OSAL_216,                       {16, 64}  },
    {OSAL_MQ_OSAL_217,                       {16, 64}  },
    {OSAL_MQ_OSAL_218,                       {16, 64}  },
    {OSAL_MQ_OSAL_219,                       {16, 64}  },
    {OSAL_MQ_OSAL_220,                       {16, 64}  },
    {OSAL_MQ_OSAL_221,                       {16, 64}  },
    {OSAL_MQ_OSAL_222,                       {16, 64}  },
    {OSAL_MQ_OSAL_223,                       {16, 64}  },
    {OSAL_MQ_OSAL_224,                       {16, 64}  },
    {OSAL_MQ_OSAL_225,                       {16, 64}  },
    {OSAL_MQ_OSAL_226,                       {16, 64}  },
    {OSAL_MQ_OSAL_227,                       {16, 64}  },
    {OSAL_MQ_OSAL_228,                       {16, 64}  },
    {OSAL_MQ_OSAL_229,                       {16, 64}  },
    {OSAL_MQ_OSAL_230,                       {16, 64}  },
    {OSAL_MQ_OSAL_231,                       {16, 64}  },
    {OSAL_MQ_OSAL_232,                       {16, 64}  },
    {OSAL_MQ_OSAL_233,                       {16, 64}  },
    {OSAL_MQ_OSAL_234,                       {16, 64}  },
    {OSAL_MQ_OSAL_235,                       {16, 64}  },
    {OSAL_MQ_OSAL_236,                       {16, 64}  },
    {OSAL_MQ_OSAL_237,                       {16, 64}  },
    {OSAL_MQ_OSAL_238,                       {16, 64}  },
    {OSAL_MQ_OSAL_239,                       {16, 64}  },
    {OSAL_MQ_OSAL_240,                       {16, 64}  },
    {OSAL_MQ_OSAL_241,                       {16, 64}  },
    {OSAL_MQ_OSAL_242,                       {16, 64}  },
    {OSAL_MQ_OSAL_243,                       {16, 64}  },
    {OSAL_MQ_OSAL_244,                       {16, 64}  },
    {OSAL_MQ_OSAL_245,                       {16, 64}  },
    {OSAL_MQ_OSAL_246,                       {16, 64}  },
    {OSAL_MQ_OSAL_247,                       {16, 64}  },
    {OSAL_MQ_OSAL_248,                       {16, 64}  },
    {OSAL_MQ_OSAL_249,                       {16, 64}  },
    {OSAL_MQ_OSAL_250,                       {16, 64}  },
    {OSAL_MQ_OSAL_251,                       {16, 64}  },
    {OSAL_MQ_OSAL_252,                       {16, 64}  },
    {OSAL_MQ_OSAL_253,                       {16, 64}  },
    {OSAL_MQ_OSAL_254,                       {16, 64}  },
    {OSAL_MQ_OSAL_255,                       {16, 64}  },
    {OSAL_MQ_OSAL_256,                       {16, 64}  },
    {OSAL_MQ_OSAL_257,                       {16, 64}  },
    {OSAL_MQ_OSAL_258,                       {16, 64}  },
    {OSAL_MQ_OSAL_259,                       {16, 64}  },
    {OSAL_MQ_OSAL_260,                       {16, 64}  },
    {OSAL_MQ_OSAL_261,                       {16, 64}  },
    {OSAL_MQ_OSAL_262,                       {16, 64}  },
    {OSAL_MQ_OSAL_263,                       {16, 64}  },
    {OSAL_MQ_OSAL_264,                       {16, 64}  },
    {OSAL_MQ_OSAL_265,                       {16, 64}  },
    {OSAL_MQ_OSAL_266,                       {16, 64}  },
    {OSAL_MQ_OSAL_267,                       {16, 64}  },
    {OSAL_MQ_OSAL_268,                       {16, 64}  },
    {OSAL_MQ_OSAL_269,                       {16, 64}  },
    {OSAL_MQ_OSAL_270,                       {16, 64}  },
    {OSAL_MQ_OSAL_271,                       {16, 64}  },
    {OSAL_MQ_OSAL_272,                       {16, 64}  },
    {OSAL_MQ_OSAL_273,                       {16, 64}  },
    {OSAL_MQ_OSAL_274,                       {16, 64}  },
    {OSAL_MQ_OSAL_275,                       {16, 64}  },
    {OSAL_MQ_OSAL_276,                       {16, 64}  },
    {OSAL_MQ_OSAL_277,                       {16, 64}  },
    {OSAL_MQ_OSAL_278,                       {16, 64}  },
    {OSAL_MQ_OSAL_279,                       {16, 64}  },
    {OSAL_MQ_OSAL_280,                       {16, 64}  },
    {OSAL_MQ_OSAL_281,                       {16, 64}  },
    {OSAL_MQ_OSAL_282,                       {16, 64}  },
    {OSAL_MQ_OSAL_283,                       {16, 64}  },
    {OSAL_MQ_OSAL_284,                       {16, 64}  },
    {OSAL_MQ_OSAL_285,                       {16, 64}  },
    {OSAL_MQ_OSAL_286,                       {16, 64}  },
    {OSAL_MQ_OSAL_287,                       {16, 64}  },
    {OSAL_MQ_OSAL_288,                       {16, 64}  },
    {OSAL_MQ_OSAL_289,                       {16, 64}  },
    {OSAL_MQ_OSAL_290,                       {16, 64}  },
    {OSAL_MQ_OSAL_291,                       {16, 64}  },
    {OSAL_MQ_OSAL_292,                       {16, 64}  },
    {OSAL_MQ_OSAL_293,                       {16, 64}  },
    {OSAL_MQ_OSAL_294,                       {16, 64}  },
    {OSAL_MQ_OSAL_295,                       {16, 64}  },
    {OSAL_MQ_OSAL_296,                       {16, 64}  },
    {OSAL_MQ_OSAL_297,                       {16, 64}  },
    {OSAL_MQ_OSAL_298,                       {16, 64}  },
    {OSAL_MQ_OSAL_299,                       {16, 64}  },
    {OSAL_MQ_OSAL_300,                       {16, 64}  },
    {OSAL_MQ_OSAL_301,                       {16, 64}  },
    {OSAL_MQ_OSAL_302,                       {16, 64}  },
    {OSAL_MQ_OSAL_303,                       {16, 64}  },
    {OSAL_MQ_OSAL_304,                       {16, 64}  },
    {OSAL_MQ_OSAL_305,                       {16, 64}  },
    {OSAL_MQ_OSAL_306,                       {16, 64}  },
    {OSAL_MQ_OSAL_307,                       {16, 64}  },
    {OSAL_MQ_OSAL_308,                       {16, 64}  },
    {OSAL_MQ_OSAL_309,                       {16, 64}  },
    {OSAL_MQ_OSAL_310,                       {16, 64}  },
    {OSAL_MQ_OSAL_311,                       {16, 64}  },
    {OSAL_MQ_OSAL_312,                       {16, 64}  },
    {OSAL_MQ_OSAL_313,                       {16, 64}  },
    {OSAL_MQ_OSAL_314,                       {16, 64}  },
    {OSAL_MQ_OSAL_315,                       {16, 64}  },
    {OSAL_MQ_OSAL_316,                       {16, 64}  },
    {OSAL_MQ_OSAL_317,                       {16, 64}  },
    {OSAL_MQ_OSAL_318,                       {16, 64}  },
    {OSAL_MQ_OSAL_319,                       {16, 64}  },
    {OSAL_MQ_OSAL_320,                       {16, 64}  },
    {OSAL_MQ_OSAL_321,                       {16, 64}  },
    {OSAL_MQ_OSAL_322,                       {16, 64}  },
    {OSAL_MQ_OSAL_323,                       {16, 64}  },
    {OSAL_MQ_OSAL_324,                       {16, 64}  },
    {OSAL_MQ_OSAL_325,                       {16, 64}  },
    {OSAL_MQ_OSAL_326,                       {16, 64}  },
    {OSAL_MQ_OSAL_327,                       {16, 64}  },
    {OSAL_MQ_OSAL_328,                       {16, 64}  },
    {OSAL_MQ_OSAL_329,                       {16, 64}  },
    {OSAL_MQ_OSAL_330,                       {16, 64}  },
    {OSAL_MQ_OSAL_331,                       {16, 64}  },
    {OSAL_MQ_OSAL_332,                       {16, 64}  },
    {OSAL_MQ_OSAL_333,                       {16, 64}  },
    {OSAL_MQ_OSAL_334,                       {16, 64}  },
    {OSAL_MQ_OSAL_335,                       {16, 64}  },
    {OSAL_MQ_OSAL_336,                       {16, 64}  },
    {OSAL_MQ_OSAL_337,                       {16, 64}  },
    {OSAL_MQ_OSAL_338,                       {16, 64}  },
    {OSAL_MQ_OSAL_339,                       {16, 64}  },
    {OSAL_MQ_OSAL_340,                       {16, 64}  },
    {OSAL_MQ_OSAL_341,                       {16, 64}  },
    {OSAL_MQ_OSAL_342,                       {16, 64}  },
    {OSAL_MQ_OSAL_343,                       {16, 64}  },
    {OSAL_MQ_OSAL_344,                       {16, 64}  },
    {OSAL_MQ_OSAL_345,                       {16, 64}  },
    {OSAL_MQ_OSAL_346,                       {16, 64}  },
    {OSAL_MQ_OSAL_347,                       {16, 64}  },
    {OSAL_MQ_OSAL_348,                       {16, 64}  },
    {OSAL_MQ_OSAL_349,                       {16, 64}  },
    {OSAL_MQ_OSAL_350,                       {16, 64}  },
    {OSAL_MQ_OSAL_351,                       {16, 64}  },
    {OSAL_MQ_OSAL_352,                       {16, 64}  },
    {OSAL_MQ_OSAL_353,                       {16, 64}  },
    {OSAL_MQ_OSAL_354,                       {16, 64}  },
    {OSAL_MQ_OSAL_355,                       {16, 64}  },
    {OSAL_MQ_OSAL_356,                       {16, 64}  },
    {OSAL_MQ_OSAL_357,                       {16, 64}  },
    {OSAL_MQ_OSAL_358,                       {16, 64}  },
    {OSAL_MQ_OSAL_359,                       {16, 64}  },
    {OSAL_MQ_OSAL_360,                       {16, 64}  },
    {OSAL_MQ_OSAL_361,                       {16, 64}  },
    {OSAL_MQ_OSAL_362,                       {16, 64}  },
    {OSAL_MQ_OSAL_363,                       {16, 64}  },
    {OSAL_MQ_OSAL_364,                       {16, 64}  },
    {OSAL_MQ_OSAL_365,                       {16, 64}  },
    {OSAL_MQ_OSAL_366,                       {16, 64}  },
    {OSAL_MQ_OSAL_367,                       {16, 64}  },
    {OSAL_MQ_OSAL_368,                       {16, 64}  },
    {OSAL_MQ_OSAL_369,                       {16, 64}  },
    {OSAL_MQ_OSAL_370,                       {16, 64}  },
    {OSAL_MQ_OSAL_371,                       {16, 64}  },
    {OSAL_MQ_OSAL_372,                       {16, 64}  },
    {OSAL_MQ_OSAL_373,                       {16, 64}  },
    {OSAL_MQ_OSAL_374,                       {16, 64}  },
    {OSAL_MQ_OSAL_375,                       {16, 64}  },
    {OSAL_MQ_OSAL_376,                       {16, 64}  },
    {OSAL_MQ_OSAL_377,                       {16, 64}  },
    {OSAL_MQ_OSAL_378,                       {16, 64}  },
    {OSAL_MQ_OSAL_379,                       {16, 64}  },
    {OSAL_MQ_OSAL_380,                       {16, 64}  },
    {OSAL_MQ_OSAL_381,                       {16, 64}  },
    {OSAL_MQ_OSAL_382,                       {16, 64}  },
    {OSAL_MQ_OSAL_383,                       {16, 64}  },
    {OSAL_MQ_OSAL_384,                       {16, 64}  },
    {OSAL_MQ_OSAL_385,                       {16, 64}  },
    {OSAL_MQ_OSAL_386,                       {16, 64}  },
    {OSAL_MQ_OSAL_387,                       {16, 64}  },
    {OSAL_MQ_OSAL_388,                       {16, 64}  },
    {OSAL_MQ_OSAL_389,                       {16, 64}  },
    {OSAL_MQ_OSAL_390,                       {16, 64}  },
    {OSAL_MQ_OSAL_391,                       {16, 64}  },
    {OSAL_MQ_OSAL_392,                       {16, 64}  },
    {OSAL_MQ_OSAL_393,                       {16, 64}  },
    {OSAL_MQ_OSAL_394,                       {16, 64}  },
    {OSAL_MQ_OSAL_395,                       {16, 64}  },
    {OSAL_MQ_OSAL_396,                       {16, 64}  },
    {OSAL_MQ_OSAL_397,                       {16, 64}  },
    {OSAL_MQ_OSAL_398,                       {16, 64}  },
    {OSAL_MQ_OSAL_399,                       {16, 64}  },
    {OSAL_MQ_OSAL_400,                       {16, 64}  },
    {OSAL_MQ_OSAL_401,                       {16, 64}  },
    {OSAL_MQ_OSAL_402,                       {16, 64}  },
    {OSAL_MQ_OSAL_403,                       {16, 64}  },
    {OSAL_MQ_OSAL_404,                       {16, 64}  },
    {OSAL_MQ_OSAL_405,                       {16, 64}  },
    {OSAL_MQ_OSAL_406,                       {16, 64}  },
    {OSAL_MQ_OSAL_407,                       {16, 64}  },
    {OSAL_MQ_OSAL_408,                       {16, 64}  },
    {OSAL_MQ_OSAL_409,                       {16, 64}  },
    {OSAL_MQ_OSAL_410,                       {16, 64}  },
    {OSAL_MQ_OSAL_411,                       {16, 64}  },
    {OSAL_MQ_OSAL_412,                       {16, 64}  },
    {OSAL_MQ_OSAL_413,                       {16, 64}  },
    {OSAL_MQ_OSAL_414,                       {16, 64}  },
    {OSAL_MQ_OSAL_415,                       {16, 64}  },
    {OSAL_MQ_OSAL_416,                       {16, 64}  },
    {OSAL_MQ_OSAL_417,                       {16, 64}  },
    {OSAL_MQ_OSAL_418,                       {16, 64}  },
    {OSAL_MQ_OSAL_419,                       {16, 64}  },
    {OSAL_MQ_OSAL_420,                       {16, 64}  },
    {OSAL_MQ_OSAL_421,                       {16, 64}  },
    {OSAL_MQ_OSAL_422,                       {16, 64}  },
    {OSAL_MQ_OSAL_423,                       {16, 64}  },
    {OSAL_MQ_OSAL_424,                       {16, 64}  },
    {OSAL_MQ_OSAL_425,                       {16, 64}  },
    {OSAL_MQ_OSAL_426,                       {16, 64}  },
    {OSAL_MQ_OSAL_427,                       {16, 64}  },
    {OSAL_MQ_OSAL_428,                       {16, 64}  },
    {OSAL_MQ_OSAL_429,                       {16, 64}  },
    {OSAL_MQ_OSAL_430,                       {16, 64}  },
    {OSAL_MQ_OSAL_431,                       {16, 64}  },
    {OSAL_MQ_OSAL_432,                       {16, 64}  },
    {OSAL_MQ_OSAL_433,                       {16, 64}  },
    {OSAL_MQ_OSAL_434,                       {16, 64}  },
    {OSAL_MQ_OSAL_435,                       {16, 64}  },
    {OSAL_MQ_OSAL_436,                       {16, 64}  },
    {OSAL_MQ_OSAL_437,                       {16, 64}  },
    {OSAL_MQ_OSAL_438,                       {16, 64}  },
    {OSAL_MQ_OSAL_439,                       {16, 64}  },
    {OSAL_MQ_OSAL_440,                       {16, 64}  },
    {OSAL_MQ_OSAL_441,                       {16, 64}  },
    {OSAL_MQ_OSAL_442,                       {16, 64}  },
    {OSAL_MQ_OSAL_443,                       {16, 64}  },
    {OSAL_MQ_OSAL_444,                       {16, 64}  },
    {OSAL_MQ_OSAL_445,                       {16, 64}  },
    {OSAL_MQ_OSAL_446,                       {16, 64}  },
    {OSAL_MQ_OSAL_447,                       {16, 64}  },
    {OSAL_MQ_OSAL_448,                       {16, 64}  },
    {OSAL_MQ_OSAL_449,                       {16, 64}  },
    {OSAL_MQ_OSAL_450,                       {16, 64}  },
    {OSAL_MQ_OSAL_451,                       {16, 64}  },
    {OSAL_MQ_OSAL_452,                       {16, 64}  },
    {OSAL_MQ_OSAL_453,                       {16, 64}  },
    {OSAL_MQ_OSAL_454,                       {16, 64}  },
    {OSAL_MQ_OSAL_455,                       {16, 64}  },
    {OSAL_MQ_OSAL_456,                       {16, 64}  },
    {OSAL_MQ_OSAL_457,                       {16, 64}  },
    {OSAL_MQ_OSAL_458,                       {16, 64}  },
    {OSAL_MQ_OSAL_459,                       {16, 64}  },
    {OSAL_MQ_OSAL_460,                       {16, 64}  },
    {OSAL_MQ_OSAL_461,                       {16, 64}  },
    {OSAL_MQ_OSAL_462,                       {16, 64}  },
    {OSAL_MQ_OSAL_463,                       {16, 64}  },
    {OSAL_MQ_OSAL_464,                       {16, 64}  },
    {OSAL_MQ_OSAL_465,                       {16, 64}  },
    {OSAL_MQ_OSAL_466,                       {16, 64}  },
    {OSAL_MQ_OSAL_467,                       {16, 64}  },
    {OSAL_MQ_OSAL_468,                       {16, 64}  },
    {OSAL_MQ_OSAL_469,                       {16, 64}  },
    {OSAL_MQ_OSAL_470,                       {16, 64}  },
    {OSAL_MQ_OSAL_471,                       {16, 64}  },
    {OSAL_MQ_OSAL_472,                       {16, 64}  },
    {OSAL_MQ_OSAL_473,                       {16, 64}  },
    {OSAL_MQ_OSAL_474,                       {16, 64}  },
    {OSAL_MQ_OSAL_475,                       {16, 64}  },
    {OSAL_MQ_OSAL_476,                       {16, 64}  },
    {OSAL_MQ_OSAL_477,                       {16, 64}  },
    {OSAL_MQ_OSAL_478,                       {16, 64}  },
    {OSAL_MQ_OSAL_479,                       {16, 64}  },
    {OSAL_MQ_OSAL_480,                       {16, 64}  },
    {OSAL_MQ_OSAL_481,                       {16, 64}  },
    {OSAL_MQ_OSAL_482,                       {16, 64}  },
    {OSAL_MQ_OSAL_483,                       {16, 64}  },
    {OSAL_MQ_OSAL_484,                       {16, 64}  },
    {OSAL_MQ_OSAL_485,                       {16, 64}  },
    {OSAL_MQ_OSAL_486,                       {16, 64}  },
    {OSAL_MQ_OSAL_487,                       {16, 64}  },
    {OSAL_MQ_OSAL_488,                       {16, 64}  },
    {OSAL_MQ_OSAL_489,                       {16, 64}  },
    {OSAL_MQ_OSAL_490,                       {16, 64}  },
    {OSAL_MQ_OSAL_491,                       {16, 64}  },
    {OSAL_MQ_OSAL_492,                       {16, 64}  },
    {OSAL_MQ_OSAL_493,                       {16, 64}  },
    {OSAL_MQ_OSAL_494,                       {16, 64}  },
    {OSAL_MQ_OSAL_495,                       {16, 64}  },
    {OSAL_MQ_OSAL_496,                       {16, 64}  },
    {OSAL_MQ_OSAL_497,                       {16, 64}  },
    {OSAL_MQ_OSAL_498,                       {16, 64}  },
    {OSAL_MQ_OSAL_499,                       {16, 64}  },
    {OSAL_MQ_OSAL_500,                       { 1,  4}  },
#endif

    {.id = OSAL_MQ_INVALID_ID}
};
#endif

/*******************************************************************************************************************//**
 * @var g_osal_interrupt_thread_config
 * osal user configuration : interrupt thread configuration table
***********************************************************************************************************************/
#ifndef TEST_LINUX
#if defined(RCAR_V3H1)  /* V3H1 */
const st_osal_interrupt_thread_config_t  g_osal_interrupt_thread_config[] =
{
    /* irq, device_id               interrupt priority            task_name,              stack_size,  interrupt_timeout */
    {192, "ims_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task000",           0x0400,      60000000},
    {193, "ims_01",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task001",           0x0400,      60000000},
    {194, "ims_02",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task002",           0x0400,      60000000},
    {195, "ims_03",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task003",           0x0400,      60000000},
    {254, "imr_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task004",           0x0400,      60000000},
    {255, "imr_01",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task005",           0x0400,      60000000},
    {242, "dof_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task006",           0x0400,      60000000},
    {243, "dof_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task007",           0x0400,      60000000},
    {238, "stv_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task010",           0x0400,      60000000},
    {239, "stv_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task011",           0x0400,      60000000},
    {71,  "acf_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task014",           0x0400,      60000000},
    {97,  "acf_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task015",           0x0400,      60000000},
    {98,  "acf_01",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task016",           0x0400,      60000000},
    {99,  "acf_01",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task017",           0x0400,      60000000},
    {100, "acf_02",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task018",           0x0400,      60000000},
    {101, "acf_02",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task019",           0x0400,      60000000},
    {102, "acf_03",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task020",           0x0400,      60000000},
    {103, "acf_03",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task021",           0x0400,      60000000},
    {104, "acf_04",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task022",           0x0400,      60000000},
    {108, "acf_04",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task023",           0x0400,      60000000},
    {224, "iccom_00",               OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task024",           0x0400,      60000000},
    {225, "iccom_01",               OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task025",           0x0400,      60000000},
    {226, "iccom_02",               OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task026",           0x0400,      60000000},
    {227, "iccom_03",               OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task027",           0x0400,      60000000},
    {228, "iccom_04",               OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task028",           0x0400,      60000000},
    {229, "iccom_05",               OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task029",           0x0400,      60000000},
    {230, "iccom_06",               OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task030",           0x0400,      60000000},
    {231, "iccom_07",               OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task031",           0x0400,      60000000},
    {300, "ivcp1e_00",              OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task032",           0x0400,      60000000},
    {170, "ivcp1e_00",              OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task033",           0x0400,      60000000},
    {281, "imp_top_00",             OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task036",           0x2000,      60000000},
    {282, "imp_top_00",             OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task037",           0x2000,      60000000},
    {283, "imp_top_00",             OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task038",           0x2000,      60000000},
    {284, "imp_top_00",             OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task039",           0x2000,      60000000},
    {285, "imp_top_00",             OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task040",           0x2000,      60000000},
    {26,  "cisp_00",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task054",           0x1000,      60000000},
    {109, "cisp_01",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task055",           0x1000,      60000000},
    {25,  "tisp_00",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task058",           0x1000,      60000000},
    {27,  "tisp_01",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task059",           0x1000,      60000000},
    {14,  "fbc_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_taskXXX",           0x1000,      60000000},
    {15,  "fbc_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_taskXXX",           0x1000,      60000000},
    {32,  "rfso_00",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task066",           0x1000,      60000000},
    {33,  "rfso_01",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task067",           0x1000,      60000000},
    {34,  "rfso_02",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task068",           0x1000,      60000000},
    {35,  "rfso_03",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task069",           0x1000,      60000000},
    {36,  "rfso_04",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task070",           0x1000,      60000000},
    {37,  "rfso_05",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task071",           0x1000,      60000000},
    {221, "rfso_06",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task072",           0x1000,      60000000},
    {186, "rfso_07",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task073",           0x1000,      60000000},
    {187, "rfso_08",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task074",           0x1000,      60000000},
    {185, "rfso_09",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task075",           0x1000,      60000000},
    {247, "rfso_10",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task076",           0x1000,      60000000},
    {196, "ipmmu_mm_00",            OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task_DUMMY",        0x1000,      60000000},

    {.id = OSAL_INTERRUPT_INVALID_ID}
};
#elif defined(RCAR_V3H2)    /* V3H2 */
const st_osal_interrupt_thread_config_t  g_osal_interrupt_thread_config[] =
{
    /* irq, device_id               interrupt priority            task_name,              stack_size,  interrupt_timeout */
    {192, "ims_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task000",           0x0400,      60000000},
    {193, "ims_01",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task001",           0x0400,      60000000},
    {194, "ims_02",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task002",           0x0400,      60000000},
    {254, "imr_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task003",           0x0400,      60000000},
    {255, "imr_01",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task004",           0x0400,      60000000},
    {242, "dof_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task006",           0x0400,      60000000},
    {243, "dof_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task007",           0x0400,      60000000},
    {238, "stv_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task010",           0x0400,      60000000},
    {239, "stv_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task011",           0x0400,      60000000},
    {71,  "acf_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task014",           0x0400,      60000000},
    {97,  "acf_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task015",           0x0400,      60000000},
    {98,  "acf_01",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task016",           0x0400,      60000000},
    {99,  "acf_01",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task017",           0x0400,      60000000},
    {100, "acf_02",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task018",           0x0400,      60000000},
    {101, "acf_02",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task019",           0x0400,      60000000},
    {102, "acf_03",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task020",           0x0400,      60000000},
    {103, "acf_03",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task021",           0x0400,      60000000},
    {104, "acf_04",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task022",           0x0400,      60000000},
    {108, "acf_04",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task023",           0x0400,      60000000},
    {224, "iccom_00",               OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task024",           0x0400,      60000000},
    {225, "iccom_01",               OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task025",           0x0400,      60000000},
    {226, "iccom_02",               OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task026",           0x0400,      60000000},
    {227, "iccom_03",               OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task027",           0x0400,      60000000},
    {228, "iccom_04",               OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task028",           0x0400,      60000000},
    {229, "iccom_05",               OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task029",           0x0400,      60000000},
    {230, "iccom_06",               OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task030",           0x0400,      60000000},
    {231, "iccom_07",               OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task031",           0x0400,      60000000},
    {300, "ivcp1e_00",              OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task032",           0x0400,      60000000},
    {170, "ivcp1e_00",              OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task033",           0x0400,      60000000},
    {281, "imp_top_00",             OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task036",           0x2000,      60000000},
    {282, "imp_top_00",             OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task037",           0x2000,      60000000},
    {283, "imp_top_00",             OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task038",           0x2000,      60000000},
    {284, "imp_top_00",             OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task039",           0x2000,      60000000},
    {285, "imp_top_00",             OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task040",           0x2000,      60000000},
    {26,  "cisp_00",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task054",           0x1000,      60000000},
    {109, "cisp_01",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task055",           0x1000,      60000000},
    {25,  "tisp_00",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task058",           0x1000,      60000000},
    {27,  "tisp_01",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task059",           0x1000,      60000000},
    {14,  "fbc_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_taskXXX",           0x1000,      60000000},
    {15,  "fbc_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_taskXXX",           0x1000,      60000000},
    {32,  "rfso_00",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task066",           0x1000,      60000000},
    {33,  "rfso_01",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task067",           0x1000,      60000000},
    {34,  "rfso_02",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task068",           0x1000,      60000000},
    {35,  "rfso_03",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task069",           0x1000,      60000000},
    {36,  "rfso_04",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task070",           0x1000,      60000000},
    {37,  "rfso_05",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task071",           0x1000,      60000000},
    {221, "rfso_06",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task072",           0x1000,      60000000},
    {186, "rfso_07",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task073",           0x1000,      60000000},
    {187, "rfso_08",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task074",           0x1000,      60000000},
    {185, "rfso_09",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task075",           0x1000,      60000000},
    {247, "rfso_10",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task076",           0x1000,      60000000},
    {196, "ipmmu_mm_00",            OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task_DUMMY",        0x1000,      60000000},

    {.id = OSAL_INTERRUPT_INVALID_ID}
};
#elif defined(RCAR_V3M)     /* V3M */
const st_osal_interrupt_thread_config_t  g_osal_interrupt_thread_config[] =
{
    /* irq, device_id               interrupt priority            task_name,              stack_size,  interrupt_timeout */
    {192, "ims_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task000",           0x0400,      60000000},
    {193, "ims_01",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task001",           0x0400,      60000000},
    {194, "ims_02",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task002",           0x0400,      60000000},
    {195, "ims_03",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task003",           0x0400,      60000000},
    {224, "iccom_00",               OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task024",           0x0400,      60000000},
    {225, "iccom_01",               OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task025",           0x0400,      60000000},
    {226, "iccom_02",               OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task026",           0x0400,      60000000},
    {227, "iccom_03",               OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task027",           0x0400,      60000000},
    {228, "iccom_04",               OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task028",           0x0400,      60000000},
    {229, "iccom_05",               OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task029",           0x0400,      60000000},
    {230, "iccom_06",               OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task030",           0x0400,      60000000},
    {231, "iccom_07",               OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task031",           0x0400,      60000000},
    {300, "ivcp1e_00",              OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task032",           0x0400,      60000000},
    {170, "ivcp1e_00",              OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task033",           0x0400,      60000000},
    {281, "imp_top_00",             OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task036",           0x2000,      60000000},
    {282, "imp_top_00",             OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task037",           0x2000,      60000000},
    {283, "imp_top_00",             OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task038",           0x2000,      60000000},
    {284, "imp_top_00",             OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task039",           0x2000,      60000000},
    {285, "imp_top_00",             OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task040",           0x2000,      60000000},
    {26,  "cisp_00",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task054",           0x1000,      60000000},
    {25,  "tisp_00",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task058",           0x1000,      60000000},
    {14,  "fbc_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_taskXXX",           0x1000,      60000000},
    {15,  "fbc_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_taskXXX",           0x1000,      60000000},
    {32,  "rfso_00",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task066",           0x1000,      60000000},
    {33,  "rfso_01",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task067",           0x1000,      60000000},
    {34,  "rfso_02",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task068",           0x1000,      60000000},
    {35,  "rfso_03",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task069",           0x1000,      60000000},
    {36,  "rfso_04",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task070",           0x1000,      60000000},
    {37,  "rfso_05",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task071",           0x1000,      60000000},
    {221, "rfso_06",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task072",           0x1000,      60000000},
    {186, "rfso_07",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task073",           0x1000,      60000000},
    {187, "rfso_08",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task074",           0x1000,      60000000},
    {185, "rfso_09",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task075",           0x1000,      60000000},
    {247, "rfso_10",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task076",           0x1000,      60000000},
    {196, "ipmmu_mm_00",            OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task_DUMMY",        0x1000,      60000000},

    {.id = OSAL_INTERRUPT_INVALID_ID}
};
#elif defined(RCAR_V3U)     /* V3U */
const st_osal_interrupt_thread_config_t  g_osal_interrupt_thread_config[] =
{
    /* irq, device_id               interrupt priority            task_name,              stack_size,  interrupt_timeout */
    {150, "ims_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task000",           0x0400,      60000000},
    {151, "ims_01",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task001",           0x0400,      60000000},
    {146, "imr_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task002",           0x0400,      60000000},
    {147, "imr_01",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task003",           0x0400,      60000000},
    {148, "imr_02",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task004",           0x0400,      60000000},
    {149, "imr_03",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task005",           0x0400,      60000000},
    {570, "dof_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task007",           0x0400,      60000000},
    {571, "dof_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task008",           0x0400,      60000000},
    {572, "dof_01",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task009",           0x0400,      60000000},
    {573, "dof_01",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task010",           0x0400,      60000000},
    {554, "stv_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task011",           0x0400,      60000000},
    {555, "stv_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task012",           0x0400,      60000000},
    {556, "stv_01",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task013",           0x0400,      60000000},
    {557, "stv_01",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task014",           0x0400,      60000000},
    {544, "acf_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task015",           0x0400,      60000000},
    {545, "acf_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task016",           0x0400,      60000000},
    {546, "acf_01",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task017",           0x0400,      60000000},
    {547, "acf_01",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task018",           0x0400,      60000000},
    {548, "acf_02",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task019",           0x0400,      60000000},
    {549, "acf_02",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task010",           0x0400,      60000000},
    {550, "acf_03",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task020",           0x0400,      60000000},
    {551, "acf_03",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task021",           0x0400,      60000000},
    {440, "iccom_00",               OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task024",           0x0400,      60000000},
    {441, "iccom_00",               OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task025",           0x0400,      60000000},
    {442, "iccom_00",               OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task026",           0x0400,      60000000},
    {443, "iccom_00",               OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task027",           0x0400,      60000000},
    {444, "iccom_00",               OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task028",           0x0400,      60000000},
    {445, "iccom_00",               OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task029",           0x0400,      60000000},
    {446, "iccom_00",               OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task030",           0x0400,      60000000},
    {447, "iccom_00",               OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task031",           0x0400,      60000000},
    {133, "ivcp1e_00",              OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task032",           0x0400,      60000000},
    {134, "ivcp1e_00",              OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task033",           0x0400,      60000000},
    {574, "vcp4_00",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task034",           0x0400,      60000000},
    {575, "vcp4_00",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task035",           0x0400,      60000000},
    {576, "imp_top_00",             OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task036",           0x2000,      60000000},
    {577, "imp_top_00",             OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task037",           0x2000,      60000000},
    {578, "imp_top_00",             OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task038",           0x2000,      60000000},
    {579, "imp_top_00",             OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task039",           0x2000,      60000000},
    {580, "imp_top_00",             OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task040",           0x2000,      60000000},
    {581, "imp_top_00",             OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task041",           0x2000,      60000000},
    {582, "imp_top_00",             OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task042",           0x2000,      60000000},
    {584, "imp_ldmac_master_00",    OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task043",           0x2000,      60000000},
    {585, "imp_radsp_00",           OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task044",           0x2000,      60000000},
    {586, "imp_radsp_01",           OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task045",           0x2000,      60000000},
    {880, "imp_radsp_dmac_00",      OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task046",           0x2000,      60000000},
    {881, "imp_radsp_dmac_00",      OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task047",           0x2000,      60000000},
    {882, "imp_radsp_dmac_00",      OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task048",           0x2000,      60000000},
    {883, "imp_radsp_dmac_00",      OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task049",           0x2000,      60000000},
    {896, "imp_radsp_dmac_01",      OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task050",           0x2000,      60000000},
    {897, "imp_radsp_dmac_01",      OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task051",           0x2000,      60000000},
    {898, "imp_radsp_dmac_01",      OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task052",           0x2000,      60000000},
    {899, "imp_radsp_dmac_01",      OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task053",           0x2000,      60000000},
    {153, "cisp_00",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task054",           0x1000,      60000000},
    {155, "cisp_01",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task055",           0x1000,      60000000},
    {157, "cisp_02",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task056",           0x1000,      60000000},
    {159, "cisp_03",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task057",           0x1000,      60000000},
    {152, "tisp_00",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task058",           0x1000,      60000000},
    {154, "tisp_01",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task059",           0x1000,      60000000},
    {156, "tisp_02",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task060",           0x1000,      60000000},
    {158, "tisp_03",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task061",           0x1000,      60000000},
    {600, "vspx_00",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task062",           0x1000,      60000000},
    {601, "vspx_01",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task063",           0x1000,      60000000},
    {602, "vspx_02",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task064",           0x1000,      60000000},
    {603, "vspx_03",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task065",           0x1000,      60000000},
    {640, "fbc_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_taskXXX",           0x1000,      60000000},
    {641, "fbc_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_taskXXX",           0x1000,      60000000},
    {642, "fbc_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_taskXXX",           0x1000,      60000000},
    {643, "fbc_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_taskXXX",           0x1000,      60000000},
    {644, "fbc_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_taskXXX",           0x1000,      60000000},
    {645, "fbc_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_taskXXX",           0x1000,      60000000},
    {646, "fbc_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_taskXXX",           0x1000,      60000000},
    {647, "fbc_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_taskXXX",           0x1000,      60000000},
    {648, "fbc_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_taskXXX",           0x1000,      60000000},
    {649, "fbc_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_taskXXX",           0x1000,      60000000},
    {650, "fbc_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_taskXXX",           0x1000,      60000000},
    {651, "fbc_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_taskXXX",           0x1000,      60000000},
    {652, "fbc_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_taskXXX",           0x1000,      60000000},
    {653, "fbc_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_taskXXX",           0x1000,      60000000},
    {654, "fbc_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_taskXXX",           0x1000,      60000000},
    {655, "fbc_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_taskXXX",           0x1000,      60000000},
    {656, "fbc_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_taskXXX",           0x1000,      60000000},
    {657, "fbc_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_taskXXX",           0x1000,      60000000},
    {658, "fbc_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_taskXXX",           0x1000,      60000000},
    {659, "fbc_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_taskXXX",           0x1000,      60000000},
    {660, "fbc_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_taskXXX",           0x1000,      60000000},
    {661, "fbc_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_taskXXX",           0x1000,      60000000},
    {662, "fbc_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_taskXXX",           0x1000,      60000000},
    {663, "fbc_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_taskXXX",           0x1000,      60000000},
    {664, "fbc_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_taskXXX",           0x1000,      60000000},
    {665, "fbc_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_taskXXX",           0x1000,      60000000},
    {666, "fbc_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_taskXXX",           0x1000,      60000000},
    {667, "fbc_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_taskXXX",           0x1000,      60000000},
    {668, "fbc_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_taskXXX",           0x1000,      60000000},
    {669, "fbc_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_taskXXX",           0x1000,      60000000},
    {670, "fbc_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_taskXXX",           0x1000,      60000000},
    {671, "fbc_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_taskXXX",           0x1000,      60000000},
    {672, "fbc_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_taskXXX",           0x1000,      60000000},
    {673, "fbc_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_taskXXX",           0x1000,      60000000},
    {674, "fbc_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_taskXXX",           0x1000,      60000000},
    {675, "fbc_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_taskXXX",           0x1000,      60000000},
    {676, "fbc_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_taskXXX",           0x1000,      60000000},
    {677, "fbc_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_taskXXX",           0x1000,      60000000},
    {678, "fbc_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_taskXXX",           0x1000,      60000000},
    {679, "fbc_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_taskXXX",           0x1000,      60000000},
    {680, "fbc_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_taskXXX",           0x1000,      60000000},
    {681, "fbc_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_taskXXX",           0x1000,      60000000},
    {682, "fbc_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_taskXXX",           0x1000,      60000000},
    {683, "fbc_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_taskXXX",           0x1000,      60000000},
    {684, "fbc_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_taskXXX",           0x1000,      60000000},
    {685, "fbc_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_taskXXX",           0x1000,      60000000},
    {686, "fbc_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_taskXXX",           0x1000,      60000000},
    {687, "fbc_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_taskXXX",           0x1000,      60000000},
    {688, "fbc_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_taskXXX",           0x1000,      60000000},
    {689, "fbc_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_taskXXX",           0x1000,      60000000},
    {690, "fbc_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_taskXXX",           0x1000,      60000000},
    {691, "fbc_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_taskXXX",           0x1000,      60000000},
    {692, "fbc_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_taskXXX",           0x1000,      60000000},
    {693, "fbc_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_taskXXX",           0x1000,      60000000},
    {694, "fbc_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_taskXXX",           0x1000,      60000000},
    {695, "fbc_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_taskXXX",           0x1000,      60000000},
    {696, "fbc_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_taskXXX",           0x1000,      60000000},
    {697, "fbc_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_taskXXX",           0x1000,      60000000},
    {698, "fbc_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_taskXXX",           0x1000,      60000000},
    {699, "fbc_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_taskXXX",           0x1000,      60000000},
    {700, "fbc_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_taskXXX",           0x1000,      60000000},
    {701, "fbc_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_taskXXX",           0x1000,      60000000},
    {702, "fbc_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_taskXXX",           0x1000,      60000000},
    {703, "fbc_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_taskXXX",           0x1000,      60000000},
    {704, "fbc_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_taskXXX",           0x1000,      60000000},
    {705, "fbc_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_taskXXX",           0x1000,      60000000},
    {706, "fbc_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_taskXXX",           0x1000,      60000000},
    {707, "fbc_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_taskXXX",           0x1000,      60000000},
    {708, "fbc_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_taskXXX",           0x1000,      60000000},
    {709, "fbc_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_taskXXX",           0x1000,      60000000},
    {710, "fbc_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_taskXXX",           0x1000,      60000000},
    {711, "fbc_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_taskXXX",           0x1000,      60000000},
    {712, "fbc_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_taskXXX",           0x1000,      60000000},
    {713, "fbc_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_taskXXX",           0x1000,      60000000},
    {714, "fbc_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_taskXXX",           0x1000,      60000000},
    {715, "fbc_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_taskXXX",           0x1000,      60000000},
    {716, "fbc_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_taskXXX",           0x1000,      60000000},
    {717, "fbc_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_taskXXX",           0x1000,      60000000},
    {718, "fbc_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_taskXXX",           0x1000,      60000000},
    {719, "fbc_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_taskXXX",           0x1000,      60000000},
    {720, "fbc_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_taskXXX",           0x1000,      60000000},
    {721, "fbc_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_taskXXX",           0x1000,      60000000},
    {722, "fbc_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_taskXXX",           0x1000,      60000000},
    {723, "fbc_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_taskXXX",           0x1000,      60000000},
    {724, "fbc_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_taskXXX",           0x1000,      60000000},
    {725, "fbc_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_taskXXX",           0x1000,      60000000},
    {726, "fbc_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_taskXXX",           0x1000,      60000000},
    {727, "fbc_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_taskXXX",           0x1000,      60000000},
    {728, "fbc_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_taskXXX",           0x1000,      60000000},
    {729, "fbc_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_taskXXX",           0x1000,      60000000},
    {730, "fbc_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_taskXXX",           0x1000,      60000000},
    {731, "fbc_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_taskXXX",           0x1000,      60000000},
    {732, "fbc_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_taskXXX",           0x1000,      60000000},
    {733, "fbc_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_taskXXX",           0x1000,      60000000},
    {734, "fbc_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_taskXXX",           0x1000,      60000000},
    {735, "fbc_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_taskXXX",           0x1000,      60000000},
    {524, "rfso_00",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task066",           0x1000,      60000000},
    {525, "rfso_01",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task067",           0x1000,      60000000},
    {526, "rfso_02",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task068",           0x1000,      60000000},
    {527, "rfso_03",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task069",           0x1000,      60000000},
    {528, "rfso_04",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task070",           0x1000,      60000000},
    {529, "rfso_05",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task071",           0x1000,      60000000},
    {530, "rfso_06",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task072",           0x1000,      60000000},
    {531, "rfso_07",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task073",           0x1000,      60000000},
    {532, "rfso_08",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task074",           0x1000,      60000000},
    {533, "rfso_09",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task075",           0x1000,      60000000},
    {534, "rfso_10",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task076",           0x1000,      60000000},
    {21 , "ipmmu_mm_00",            OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task_DUMMY",        0x1000,      60000000},
    {566, "smd_ps_00",              OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task_DUMMY",        0x1000,      60000000},
    {567, "smd_ps_00",              OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task_DUMMY",        0x1000,      60000000},
    {568, "smd_ps_01",              OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task_DUMMY",        0x1000,      60000000},
    {569, "smd_ps_01",              OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task_DUMMY",        0x1000,      60000000},
    {562, "smd_post_00",            OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task_DUMMY",        0x1000,      60000000},
    {563, "smd_post_00",            OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task_DUMMY",        0x1000,      60000000},
    {564, "smd_post_01",            OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task_DUMMY",        0x1000,      60000000},
    {565, "smd_post_01",            OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task_DUMMY",        0x1000,      60000000},

    {.id = OSAL_INTERRUPT_INVALID_ID}
};
#elif defined(RCAR_S4) || defined(RCAR_S4N)    /* S4 and S4N */
/* PRQA S 1531 1 # It must be external linkage separately from the referenced file because of the object to be exposed to the user. */
const st_osal_interrupt_thread_config_t  g_osal_interrupt_thread_config[] =
{
    /* irq, device_id               interrupt priority            task_name,              stack_size,  interrupt_timeout */
    {933, "caiplite_00",            OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task000",           0x0400,      60000000},
    {935, "caiplite_01",            OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task001",           0x0400,      60000000},
    {937, "caiplite_02",            OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task002",           0x0400,      60000000},
    {939, "caiplite_03",            OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task003",           0x0400,      60000000},
    {941, "caiplite_04",            OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task004",           0x0400,      60000000},
    {943, "caiplite_05",            OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task005",           0x0400,      60000000},
    {945, "caiplite_06",            OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task006",           0x0400,      60000000},
    {947, "caiplite_07",            OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task007",           0x0400,      60000000},
    { 30, "rtdmac_control0_00",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task008",           0x0400,      60000000},
    { 31, "rtdmac_control0_01",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task009",           0x0400,      60000000},
    { 32, "rtdmac_control0_02",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task010",           0x0400,      60000000},
    { 33, "rtdmac_control0_03",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task011",           0x0400,      60000000},
    { 34, "rtdmac_control0_04",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task012",           0x0400,      60000000},
    { 35, "rtdmac_control0_05",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task013",           0x0400,      60000000},
    { 36, "rtdmac_control0_06",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task014",           0x0400,      60000000},
    { 37, "rtdmac_control0_07",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task015",           0x0400,      60000000},
    { 38, "rtdmac_control0_08",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task016",           0x0400,      60000000},
    { 39, "rtdmac_control0_09",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task017",           0x0400,      60000000},
    { 40, "rtdmac_control0_10",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task018",           0x0400,      60000000},
    { 41, "rtdmac_control0_11",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task019",           0x0400,      60000000},
    { 42, "rtdmac_control0_12",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task020",           0x0400,      60000000},
    { 43, "rtdmac_control0_13",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task021",           0x0400,      60000000},
    { 44, "rtdmac_control0_14",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task022",           0x0400,      60000000},
    { 45, "rtdmac_control0_15",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task023",           0x0400,      60000000},
    { 48, "rtdmac_control1_00",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task024",           0x0400,      60000000},
    { 49, "rtdmac_control1_01",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task025",           0x0400,      60000000},
    { 50, "rtdmac_control1_02",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task026",           0x0400,      60000000},
    { 51, "rtdmac_control1_03",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task027",           0x0400,      60000000},
    { 52, "rtdmac_control1_04",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task028",           0x0400,      60000000},
    { 53, "rtdmac_control1_05",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task029",           0x0400,      60000000},
    { 54, "rtdmac_control1_06",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task030",           0x0400,      60000000},
    { 55, "rtdmac_control1_07",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task031",           0x0400,      60000000},
    { 56, "rtdmac_control1_08",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task032",           0x0400,      60000000},
    { 57, "rtdmac_control1_09",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task033",           0x0400,      60000000},
    { 58, "rtdmac_control1_10",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task034",           0x0400,      60000000},
    { 59, "rtdmac_control1_11",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task035",           0x0400,      60000000},
    { 60, "rtdmac_control1_12",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task036",           0x0400,      60000000},
    { 61, "rtdmac_control1_13",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task037",           0x0400,      60000000},
    { 62, "rtdmac_control1_14",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task038",           0x0400,      60000000},
    { 63, "rtdmac_control1_15",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task039",           0x0400,      60000000},
    { 66, "rtdmac_control2_00",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task040",           0x0400,      60000000},
    { 67, "rtdmac_control2_01",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task041",           0x0400,      60000000},
    { 68, "rtdmac_control2_02",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task042",           0x0400,      60000000},
    { 69, "rtdmac_control2_03",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task043",           0x0400,      60000000},
    { 70, "rtdmac_control2_04",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task044",           0x0400,      60000000},
    { 71, "rtdmac_control2_05",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task045",           0x0400,      60000000},
    { 72, "rtdmac_control2_06",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task046",           0x0400,      60000000},
    { 73, "rtdmac_control2_07",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task047",           0x0400,      60000000},
    { 74, "rtdmac_control2_08",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task048",           0x0400,      60000000},
    { 75, "rtdmac_control2_09",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task049",           0x0400,      60000000},
    { 76, "rtdmac_control2_10",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task050",           0x0400,      60000000},
    { 77, "rtdmac_control2_11",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task051",           0x0400,      60000000},
    { 78, "rtdmac_control2_12",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task052",           0x0400,      60000000},
    { 79, "rtdmac_control2_13",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task053",           0x0400,      60000000},
    { 80, "rtdmac_control2_14",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task054",           0x0400,      60000000},
    { 81, "rtdmac_control2_15",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task055",           0x0400,      60000000},
    { 84, "rtdmac_control3_00",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task056",           0x0400,      60000000},
    { 85, "rtdmac_control3_01",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task057",           0x0400,      60000000},
    { 86, "rtdmac_control3_02",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task058",           0x0400,      60000000},
    { 87, "rtdmac_control3_03",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task059",           0x0400,      60000000},
    { 88, "rtdmac_control3_04",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task060",           0x0400,      60000000},
    { 89, "rtdmac_control3_05",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task061",           0x0400,      60000000},
    { 90, "rtdmac_control3_06",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task062",           0x0400,      60000000},
    { 91, "rtdmac_control3_07",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task063",           0x0400,      60000000},
    { 92, "rtdmac_control3_08",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task064",           0x0400,      60000000},
    { 93, "rtdmac_control3_09",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task065",           0x0400,      60000000},
    { 94, "rtdmac_control3_10",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task066",           0x0400,      60000000},
    { 95, "rtdmac_control3_11",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task067",           0x0400,      60000000},
    { 96, "rtdmac_control3_12",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task068",           0x0400,      60000000},
    { 97, "rtdmac_control3_13",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task069",           0x0400,      60000000},
    { 98, "rtdmac_control3_14",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task070",           0x0400,      60000000},
    { 99, "rtdmac_control3_15",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task071",           0x0400,      60000000},
    {948, "ships_00",               OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task072",           0x0400,      60000000},
    {949, "ships_00",               OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task073",           0x0400,      60000000},
    {950, "ships_01",               OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task074",           0x0400,      60000000},
    {951, "ships_01",               OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task075",           0x0400,      60000000},
    {952, "ships_02",               OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task076",           0x0400,      60000000},
    {953, "ships_02",               OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task077",           0x0400,      60000000},
    {320, "iccom_00",               OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task078",           0x0400,      60000000},
    {322, "iccom_01",               OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task079",           0x0400,      60000000},
    {324, "iccom_02",               OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task080",           0x0400,      60000000},
    {326, "iccom_03",               OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task081",           0x0400,      60000000},
    {328, "iccom_04",               OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task082",           0x0400,      60000000},
    {330, "iccom_05",               OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task083",           0x0400,      60000000},
    {332, "iccom_06",               OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task084",           0x0400,      60000000},
    {334, "iccom_07",               OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task085",           0x0400,      60000000},
    {336, "iccom_08",               OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task086",           0x0400,      60000000},
    {338, "iccom_09",               OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task087",           0x0400,      60000000},
    {340, "iccom_10",               OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task088",           0x0400,      60000000},
    {342, "iccom_11",               OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task089",           0x0400,      60000000},
    {344, "iccom_12",               OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task090",           0x0400,      60000000},
    {346, "iccom_13",               OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task091",           0x0400,      60000000},
    {348, "iccom_14",               OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task092",           0x0400,      60000000},
    {350, "iccom_15",               OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task093",           0x0400,      60000000},
    {354, "iccom_16",               OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task094",           0x0400,      60000000},
    {356, "iccom_17",               OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task095",           0x0400,      60000000},
    {358, "iccom_18",               OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task096",           0x0400,      60000000},
    {360, "iccom_19",               OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task097",           0x0400,      60000000},
    {362, "iccom_20",               OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task098",           0x0400,      60000000},
    {364, "iccom_21",               OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task099",           0x0400,      60000000},
    {366, "iccom_22",               OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task100",           0x0400,      60000000},
    {368, "iccom_23",               OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task101",           0x0400,      60000000},
    {192, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task192",           0x0400,      60000000},
    {193, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task193",           0x0400,      60000000},
    {194, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task194",           0x0400,      60000000},
    {195, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task195",           0x0400,      60000000},
    {196, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task196",           0x0400,      60000000},
    {197, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task197",           0x0400,      60000000},
    {198, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task198",           0x0400,      60000000},
    {199, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task199",           0x0400,      60000000},
    {200, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task200",           0x0400,      60000000},
    {201, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task201",           0x0400,      60000000},
    {202, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task202",           0x0400,      60000000},
    {203, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task203",           0x0400,      60000000},
    {204, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task204",           0x0400,      60000000},
    {205, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task205",           0x0400,      60000000},
    {206, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task206",           0x0400,      60000000},
    {207, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task207",           0x0400,      60000000},
    {208, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task208",           0x0400,      60000000},
    {209, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task209",           0x0400,      60000000},
    {210, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task210",           0x0400,      60000000},
    {211, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task211",           0x0400,      60000000},

    {.id = OSAL_INTERRUPT_INVALID_ID}
};
#elif defined(RCAR_V4H)     /* V4H */
/* PRQA S 1531 1 # It must be external linkage separately from the referenced file because of the object to be exposed to the user. */
const st_osal_interrupt_thread_config_t  g_osal_interrupt_thread_config[] =
{
    /* irq, device_id               interrupt priority            task_name,              stack_size,  interrupt_timeout */
    {579, "ims_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task000",           0x0400,      60000000},
    {580, "ims_01",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task001",           0x0400,      60000000},
    {581, "imr_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task002",           0x0400,      60000000},
    {582, "imr_01",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task003",           0x0400,      60000000},
    {583, "imr_02",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task004",           0x0400,      60000000},
    {565, "imp_top_00",             OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task005",           0x0400,      60000000},
    {566, "imp_top_00",             OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task006",           0x0400,      60000000},
    {567, "imp_top_00",             OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task007",           0x0400,      60000000},
    {478, "vdsp00_00",              OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task008",           0x0400,      60000000},
    {479, "vdsp00_00",              OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task009",           0x0400,      60000000},
    {480, "vdsp00_00",              OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task010",           0x0400,      60000000},
    {481, "vdsp00_00",              OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task011",           0x0400,      60000000},
    {482, "vdsp00_00",              OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task012",           0x0400,      60000000},
    {483, "vdsp01_00",              OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task013",           0x0400,      60000000},
    {484, "vdsp01_00",              OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task014",           0x2000,      60000000},
    {485, "vdsp01_00",              OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task015",           0x2000,      60000000},
    {486, "vdsp01_00",              OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task017",           0x2000,      60000000},
    {487, "vdsp01_00",              OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task018",           0x2000,      60000000},
    {488, "vdsp02_00",              OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task019",           0x1000,      60000000},
    {489, "vdsp02_00",              OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task020",           0x1000,      60000000},
    {490, "vdsp02_00",              OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task021",           0x1000,      60000000},
    {491, "vdsp02_00",              OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task022",           0x1000,      60000000},
    {492, "vdsp02_00",              OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task023",           0x1000,      60000000},
    {493, "vdsp03_00",              OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task024",           0x1000,      60000000},
    {494, "vdsp03_00",              OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task025",           0x1000,      60000000},
    {495, "vdsp03_00",              OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task026",           0x1000,      60000000},
    {496, "vdsp03_00",              OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task027",           0x1000,      60000000},
    {497, "vdsp03_00",              OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task028",           0x1000,      60000000},
    {861, "dof_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task029",           0x1000,      60000000},
    {862, "dof_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task030",           0x1000,      60000000},
    {873, "smd_ps_00",              OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task031",           0x1000,      60000000},
    {874, "smd_ps_00",              OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task032",           0x1000,      60000000},
    {871, "smd_post_00",            OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task033",           0x1000,      60000000},
    {872, "smd_post_00",            OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task034",           0x1000,      60000000},
    {464, "gsx_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task035",           0x1000,      60000000},
    {123, "iccom_00",               OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task036",           0x1000,      60000000},
    {125, "iccom_01",               OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task037",           0x1000,      60000000},
    {127, "iccom_02",               OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task038",           0x1000,      60000000},
    {129, "iccom_03",               OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task039",           0x1000,      60000000},
    {131, "iccom_04",               OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task040",           0x1000,      60000000},
    {133, "iccom_05",               OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task041",           0x1000,      60000000},
    {135, "iccom_06",               OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task042",           0x1000,      60000000},
    {137, "iccom_07",               OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task043",           0x1000,      60000000},
    {916, "iccom_08",               OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task044",           0x1000,      60000000},
    {918, "iccom_09",               OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task045",           0x1000,      60000000},
    {920, "iccom_10",               OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task046",           0x1000,      60000000},
    {922, "iccom_11",               OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task047",           0x1000,      60000000},
    {855, "ivcp1e_00",              OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task048",           0x1000,      60000000},
    {856, "ivcp1e_00",              OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task049",           0x1000,      60000000},
    {858, "vcp4_00",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task050",           0x1000,      60000000},
    {859, "vcp4_00",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task051",           0x1000,      60000000},
    {475, "cisp_00",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task052",           0x1000,      60000000},
    {473, "tisp_00",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task053",           0x1000,      60000000},
    {476, "cisp_01",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task054",           0x1000,      60000000},
    {474, "tisp_01",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task055",           0x1000,      60000000},
    {556, "vspx_00",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task056",           0x1000,      60000000},
    {557, "vspx_01",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task057",           0x1000,      60000000},
    {852, "fbc_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task058",           0x1000,      60000000},
    {695, "fba_ims0",               OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task059",           0x1000,      60000000},
    {696, "fba_ims1",               OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task060",           0x1000,      60000000},
    {691, "fba_imr0",               OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task061",           0x1000,      60000000},
    {692, "fba_imr1",               OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task062",           0x1000,      60000000},
    {693, "fba_imr2",               OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task063",           0x1000,      60000000},
    {713, "fba_imp0",               OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task064",           0x1000,      60000000},
    {714, "fba_imp1",               OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task065",           0x1000,      60000000},
    {715, "fba_imp2",               OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task066",           0x1000,      60000000},
    {716, "fba_imp3",               OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task067",           0x1000,      60000000},
    {719, "fba_imp_ocv0",           OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task068",           0x1000,      60000000},
    {720, "fba_imp_ocv1",           OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task069",           0x1000,      60000000},
    {721, "fba_imp_ocv2",           OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task070",           0x1000,      60000000},
    {722, "fba_imp_ocv3",           OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task071",           0x1000,      60000000},
    {717, "fba_imp_dp0",            OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task072",           0x1000,      60000000},
    {718, "fba_imp_dp1",            OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task073",           0x1000,      60000000},
    {727, "fba_imp_cnn0",           OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task074",           0x1000,      60000000},
    {738, "fba_imp_dsp0",           OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task075",           0x1000,      60000000},
    {739, "fba_imp_dsp1",           OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task076",           0x1000,      60000000},
    {740, "fba_imp_dsp2",           OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task077",           0x1000,      60000000},
    {741, "fba_imp_dsp3",           OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task078",           0x1000,      60000000},
    {744, "fba_imp_cnram0",         OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task079",           0x1000,      60000000},
    {745, "fba_imp_slim0",          OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task080",           0x1000,      60000000},
    {746, "fba_imp_slim1",          OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task081",           0x1000,      60000000},
    {701, "fba_umfl0",              OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task082",           0x1000,      60000000},
    {703, "fba_smps0",              OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task083",           0x1000,      60000000},
    {707, "fba_smpo0",              OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task084",           0x1000,      60000000},
    {699, "fba_pap0",               OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task085",           0x1000,      60000000},
    {686, "fba_isp0",               OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task086",           0x1000,      60000000},
    {687, "fba_isp1",               OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task087",           0x1000,      60000000},
    {669, "fba_ca76d0",             OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task088",           0x1000,      60000000},
    {670, "fba_ca76d1",             OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task089",           0x1000,      60000000},
    {671, "fba_ca760",              OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task090",           0x1000,      60000000},
    {672, "fba_ca761",              OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task091",           0x1000,      60000000},
    {673, "fba_ca762",              OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task092",           0x1000,      60000000},
    {674, "fba_ca763",              OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task093",           0x1000,      60000000},
    {227, "rfso_00",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task094",           0x1000,      60000000},
    {228, "rfso_01",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task095",           0x1000,      60000000},
    {229, "rfso_02",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task096",           0x1000,      60000000},
    {230, "rfso_03",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task097",           0x1000,      60000000},
    {231, "rfso_04",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task098",           0x1000,      60000000},
    {232, "rfso_05",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task099",           0x1000,      60000000},
    {233, "rfso_06",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task100",           0x1000,      60000000},
    {234, "rfso_07",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task101",           0x1000,      60000000},
    {235, "rfso_08",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task102",           0x1000,      60000000},
    {236, "rfso_09",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task103",           0x1000,      60000000},
    {237, "rfso_10",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task104",           0x1000,      60000000},
#if !defined (TEST_EMCOS)
    {210, "ipmmu_mm_00",            OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task105",           0x1000,      60000000},
    {211, "ipmmu_mm_00",            OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task264",           0x1000,      60000000},
#endif
    {900, "caiplite_00",            OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task106",           0x1000,      60000000},
    {902, "caiplite_01",            OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task107",           0x1000,      60000000},
    {904, "caiplite_02",            OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task108",           0x1000,      60000000},
    {906, "caiplite_03",            OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task109",           0x1000,      60000000},
    {908, "caiplite_04",            OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task110",           0x1000,      60000000},
    {910, "caiplite_05",            OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task111",           0x1000,      60000000},
    {912, "caiplite_06",            OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task112",           0x1000,      60000000},
    {914, "caiplite_07",            OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task113",           0x1000,      60000000},
#if defined (TEST_EMCOS)
    { 10, "rtdmac_control_00",      OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task114",           0x0400,      60000000},
    { 11, "rtdmac_control_00",      OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task115",           0x0400,      60000000},
    { 12, "rtdmac_control_00",      OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task116",           0x0400,      60000000},
    { 13, "rtdmac_control_00",      OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task117",           0x0400,      60000000},
    { 14, "rtdmac_control_00",      OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task118",           0x0400,      60000000},
    { 15, "rtdmac_control_00",      OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task119",           0x0400,      60000000},
    { 16, "rtdmac_control_00",      OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task120",           0x0400,      60000000},
    { 17, "rtdmac_control_00",      OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task121",           0x0400,      60000000},
    { 18, "rtdmac_control_00",      OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task122",           0x0400,      60000000},
    { 19, "rtdmac_control_00",      OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task123",           0x0400,      60000000},
    { 20, "rtdmac_control_00",      OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task124",           0x0400,      60000000},
    { 21, "rtdmac_control_00",      OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task125",           0x0400,      60000000},
    { 22, "rtdmac_control_00",      OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task126",           0x0400,      60000000},
    { 23, "rtdmac_control_00",      OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task127",           0x0400,      60000000},
    { 24, "rtdmac_control_00",      OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task128",           0x0400,      60000000},
    { 25, "rtdmac_control_00",      OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task129",           0x0400,      60000000},
    { 28, "rtdmac_control_01",      OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task130",           0x0400,      60000000},
    { 29, "rtdmac_control_01",      OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task131",           0x0400,      60000000},
    { 30, "rtdmac_control_01",      OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task132",           0x0400,      60000000},
    { 31, "rtdmac_control_01",      OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task133",           0x0400,      60000000},
    { 32, "rtdmac_control_01",      OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task134",           0x0400,      60000000},
    { 33, "rtdmac_control_01",      OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task135",           0x0400,      60000000},
    { 34, "rtdmac_control_01",      OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task136",           0x0400,      60000000},
    { 35, "rtdmac_control_01",      OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task137",           0x0400,      60000000},
    { 36, "rtdmac_control_01",      OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task138",           0x0400,      60000000},
    { 37, "rtdmac_control_01",      OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task139",           0x0400,      60000000},
    { 38, "rtdmac_control_01",      OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task140",           0x0400,      60000000},
    { 39, "rtdmac_control_01",      OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task141",           0x0400,      60000000},
    { 40, "rtdmac_control_01",      OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task142",           0x0400,      60000000},
    { 41, "rtdmac_control_01",      OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task143",           0x0400,      60000000},
    { 42, "rtdmac_control_01",      OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task144",           0x0400,      60000000},
    { 43, "rtdmac_control_01",      OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task145",           0x0400,      60000000},
    { 46, "rtdmac_control_02",      OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task146",           0x0400,      60000000},
    { 47, "rtdmac_control_02",      OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task147",           0x0400,      60000000},
    { 48, "rtdmac_control_02",      OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task148",           0x0400,      60000000},
    { 49, "rtdmac_control_02",      OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task149",           0x0400,      60000000},
    { 50, "rtdmac_control_02",      OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task150",           0x0400,      60000000},
    { 51, "rtdmac_control_02",      OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task151",           0x0400,      60000000},
    { 52, "rtdmac_control_02",      OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task152",           0x0400,      60000000},
    { 53, "rtdmac_control_02",      OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task153",           0x0400,      60000000},
    { 54, "rtdmac_control_02",      OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task154",           0x0400,      60000000},
    { 55, "rtdmac_control_02",      OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task155",           0x0400,      60000000},
    { 56, "rtdmac_control_02",      OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task156",           0x0400,      60000000},
    { 57, "rtdmac_control_02",      OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task157",           0x0400,      60000000},
    { 58, "rtdmac_control_02",      OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task158",           0x0400,      60000000},
    { 59, "rtdmac_control_02",      OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task159",           0x0400,      60000000},
    { 60, "rtdmac_control_02",      OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task160",           0x0400,      60000000},
    { 61, "rtdmac_control_02",      OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task161",           0x0400,      60000000},
    { 64, "rtdmac_control_03",      OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task162",           0x0400,      60000000},
    { 65, "rtdmac_control_03",      OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task163",           0x0400,      60000000},
    { 66, "rtdmac_control_03",      OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task164",           0x0400,      60000000},
    { 67, "rtdmac_control_03",      OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task165",           0x0400,      60000000},
    { 68, "rtdmac_control_03",      OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task166",           0x0400,      60000000},
    { 69, "rtdmac_control_03",      OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task167",           0x0400,      60000000},
    { 70, "rtdmac_control_03",      OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task168",           0x0400,      60000000},
    { 71, "rtdmac_control_03",      OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task169",           0x0400,      60000000},
    { 72, "rtdmac_control_03",      OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task170",           0x0400,      60000000},
    { 73, "rtdmac_control_03",      OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task171",           0x0400,      60000000},
    { 74, "rtdmac_control_03",      OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task172",           0x0400,      60000000},
    { 75, "rtdmac_control_03",      OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task173",           0x0400,      60000000},
    { 76, "rtdmac_control_03",      OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task174",           0x0400,      60000000},
    { 77, "rtdmac_control_03",      OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task175",           0x0400,      60000000},
    { 78, "rtdmac_control_03",      OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task176",           0x0400,      60000000},
    { 79, "rtdmac_control_03",      OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task177",           0x0400,      60000000},
#else
    { 10, "rtdmac_control0_00",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task114",           0x0400,      60000000},
    { 11, "rtdmac_control0_01",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task115",           0x0400,      60000000},
    { 12, "rtdmac_control0_02",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task116",           0x0400,      60000000},
    { 13, "rtdmac_control0_03",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task117",           0x0400,      60000000},
    { 14, "rtdmac_control0_04",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task118",           0x0400,      60000000},
    { 15, "rtdmac_control0_05",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task119",           0x0400,      60000000},
    { 16, "rtdmac_control0_06",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task120",           0x0400,      60000000},
    { 17, "rtdmac_control0_07",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task121",           0x0400,      60000000},
    { 18, "rtdmac_control0_08",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task122",           0x0400,      60000000},
    { 19, "rtdmac_control0_09",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task123",           0x0400,      60000000},
    { 20, "rtdmac_control0_10",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task124",           0x0400,      60000000},
    { 21, "rtdmac_control0_11",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task125",           0x0400,      60000000},
    { 22, "rtdmac_control0_12",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task126",           0x0400,      60000000},
    { 23, "rtdmac_control0_13",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task127",           0x0400,      60000000},
    { 24, "rtdmac_control0_14",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task128",           0x0400,      60000000},
    { 25, "rtdmac_control0_15",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task129",           0x0400,      60000000},
    { 28, "rtdmac_control1_00",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task130",           0x0400,      60000000},
    { 29, "rtdmac_control1_01",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task131",           0x0400,      60000000},
    { 30, "rtdmac_control1_02",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task132",           0x0400,      60000000},
    { 31, "rtdmac_control1_03",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task133",           0x0400,      60000000},
    { 32, "rtdmac_control1_04",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task134",           0x0400,      60000000},
    { 33, "rtdmac_control1_05",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task135",           0x0400,      60000000},
    { 34, "rtdmac_control1_06",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task136",           0x0400,      60000000},
    { 35, "rtdmac_control1_07",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task137",           0x0400,      60000000},
    { 36, "rtdmac_control1_08",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task138",           0x0400,      60000000},
    { 37, "rtdmac_control1_09",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task139",           0x0400,      60000000},
    { 38, "rtdmac_control1_10",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task140",           0x0400,      60000000},
    { 39, "rtdmac_control1_11",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task141",           0x0400,      60000000},
    { 40, "rtdmac_control1_12",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task142",           0x0400,      60000000},
    { 41, "rtdmac_control1_13",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task143",           0x0400,      60000000},
    { 42, "rtdmac_control1_14",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task144",           0x0400,      60000000},
    { 43, "rtdmac_control1_15",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task145",           0x0400,      60000000},
    { 46, "rtdmac_control2_00",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task146",           0x0400,      60000000},
    { 47, "rtdmac_control2_01",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task147",           0x0400,      60000000},
    { 48, "rtdmac_control2_02",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task148",           0x0400,      60000000},
    { 49, "rtdmac_control2_03",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task149",           0x0400,      60000000},
    { 50, "rtdmac_control2_04",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task150",           0x0400,      60000000},
    { 51, "rtdmac_control2_05",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task151",           0x0400,      60000000},
    { 52, "rtdmac_control2_06",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task152",           0x0400,      60000000},
    { 53, "rtdmac_control2_07",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task153",           0x0400,      60000000},
    { 54, "rtdmac_control2_08",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task154",           0x0400,      60000000},
    { 55, "rtdmac_control2_09",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task155",           0x0400,      60000000},
    { 56, "rtdmac_control2_10",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task156",           0x0400,      60000000},
    { 57, "rtdmac_control2_11",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task157",           0x0400,      60000000},
    { 58, "rtdmac_control2_12",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task158",           0x0400,      60000000},
    { 59, "rtdmac_control2_13",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task159",           0x0400,      60000000},
    { 60, "rtdmac_control2_14",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task160",           0x0400,      60000000},
    { 61, "rtdmac_control2_15",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task161",           0x0400,      60000000},
    { 64, "rtdmac_control3_00",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task162",           0x0400,      60000000},
    { 65, "rtdmac_control3_01",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task163",           0x0400,      60000000},
    { 66, "rtdmac_control3_02",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task164",           0x0400,      60000000},
    { 67, "rtdmac_control3_03",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task165",           0x0400,      60000000},
    { 68, "rtdmac_control3_04",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task166",           0x0400,      60000000},
    { 69, "rtdmac_control3_05",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task167",           0x0400,      60000000},
    { 70, "rtdmac_control3_06",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task168",           0x0400,      60000000},
    { 71, "rtdmac_control3_07",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task169",           0x0400,      60000000},
    { 72, "rtdmac_control3_08",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task170",           0x0400,      60000000},
    { 73, "rtdmac_control3_09",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task171",           0x0400,      60000000},
    { 74, "rtdmac_control3_10",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task172",           0x0400,      60000000},
    { 75, "rtdmac_control3_11",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task173",           0x0400,      60000000},
    { 76, "rtdmac_control3_12",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task174",           0x0400,      60000000},
    { 77, "rtdmac_control3_13",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task175",           0x0400,      60000000},
    { 78, "rtdmac_control3_14",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task176",           0x0400,      60000000},
    { 79, "rtdmac_control3_15",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task177",           0x0400,      60000000},
#endif
    {529, "vin_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task178",           0x0400,      60000000},
    {530, "vin_01",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task179",           0x0400,      60000000},
    {531, "vin_02",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task180",           0x0400,      60000000},
    {532, "vin_03",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task181",           0x0400,      60000000},
    {533, "vin_04",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task182",           0x0400,      60000000},
    {534, "vin_05",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task183",           0x0400,      60000000},
    {535, "vin_06",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task184",           0x0400,      60000000},
    {536, "vin_07",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task185",           0x0400,      60000000},
    {537, "vin_10",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task186",           0x0400,      60000000},
    {538, "vin_11",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task187",           0x0400,      60000000},
    {539, "vin_12",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task188",           0x0400,      60000000},
    {540, "vin_13",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task189",           0x0400,      60000000},
    {541, "vin_14",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task190",           0x0400,      60000000},
    {542, "vin_15",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task191",           0x0400,      60000000},
    {543, "vin_16",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task192",           0x0400,      60000000},
    {544, "vin_17",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task193",           0x0400,      60000000},
    {546, "vsp_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task194",           0x0400,      60000000},
    {551, "vsp_01",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task195",           0x0400,      60000000},
    {499, "csi_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task196",           0x0400,      60000000},
    {500, "csi_01",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task197",           0x0400,      60000000},
    {523, "du_00",                  OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task198",           0x0400,      60000000},
    {524, "du_00",                  OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task199",           0x0400,      60000000},
    {502, "dsi_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task200",           0x0400,      60000000},
    {503, "dsi_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task201",           0x0400,      60000000},
    {504, "dsi_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task202",           0x0400,      60000000},
    {505, "dsi_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task203",           0x0400,      60000000},
    {506, "dsi_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task204",           0x0400,      60000000},
    {507, "dsi_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task205",           0x0400,      60000000},
    {508, "dsi_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task206",           0x0400,      60000000},
    {509, "dsi_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task207",           0x0400,      60000000},
    {510, "dsi_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task208",           0x0400,      60000000},
    {511, "dsi_01",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task209",           0x0400,      60000000},
    {512, "dsi_01",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task210",           0x0400,      60000000},
    {513, "dsi_01",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task211",           0x0400,      60000000},
    {514, "dsi_01",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task212",           0x0400,      60000000},
    {515, "dsi_01",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task213",           0x0400,      60000000},
    {516, "dsi_01",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task214",           0x0400,      60000000},
    {517, "dsi_01",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task215",           0x0400,      60000000},
    {518, "dsi_01",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task216",           0x0400,      60000000},
    {519, "dsi_01",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task217",           0x0400,      60000000},
    {310, "wwdt_00",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task218",           0x0400,      60000000},
    {311, "wwdt_00",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task219",           0x0400,      60000000},
    {312, "wwdt_01",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task220",           0x0400,      60000000},
    {313, "wwdt_01",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task221",           0x0400,      60000000},
    {314, "wwdt_02",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task222",           0x0400,      60000000},
    {315, "wwdt_02",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task223",           0x0400,      60000000},
    {316, "wwdt_03",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task224",           0x0400,      60000000},
    {317, "wwdt_03",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task225",           0x0400,      60000000},
    {318, "wwdt_04",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task226",           0x0400,      60000000},
    {319, "wwdt_04",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task227",           0x0400,      60000000},
    {320, "wwdt_05",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task228",           0x0400,      60000000},
    {321, "wwdt_05",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task229",           0x0400,      60000000},
    {322, "wwdt_06",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task230",           0x0400,      60000000},
    {323, "wwdt_06",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task231",           0x0400,      60000000},
    {324, "wwdt_07",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task258",           0x0400,      60000000},
    {325, "wwdt_07",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task259",           0x0400,      60000000},
    {326, "wwdt_08",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task260",           0x0400,      60000000},
    {327, "wwdt_08",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task261",           0x0400,      60000000},
    {328, "wwdt_09",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task262",           0x0400,      60000000},
    {329, "wwdt_09",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task263",           0x0400,      60000000},
    {526, "doc_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task232",           0x0400,      60000000},
    {527, "doc_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task233",           0x0400,      60000000},
    {559, "dsc_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task234",           0x0400,      60000000},
    {213, "sdmac_00",               OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task235",           0x0400,      60000000},
    {214, "sdmac_00",               OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task236",           0x0400,      60000000},
    {223, "fbs_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task237",           0x0400,      60000000},
    {878, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task238",           0x0400,      60000000},
    {879, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task239",           0x0400,      60000000},
    {880, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task240",           0x0400,      60000000},
    {881, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task241",           0x0400,      60000000},
    {882, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task242",           0x0400,      60000000},
    {883, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task243",           0x0400,      60000000},
    {884, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task244",           0x0400,      60000000},
    {885, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task245",           0x0400,      60000000},
    {886, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task246",           0x0400,      60000000},
    {887, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task247",           0x0400,      60000000},
    {888, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task248",           0x0400,      60000000},
    {889, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task249",           0x0400,      60000000},
    {890, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task250",           0x0400,      60000000},
    {891, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task251",           0x0400,      60000000},
    {892, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task252",           0x0400,      60000000},
    {893, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task253",           0x0400,      60000000},
    {894, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task254",           0x0400,      60000000},
    {895, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task255",           0x0400,      60000000},
    {896, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task256",           0x0400,      60000000},
    {897, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task257",           0x0400,      60000000},

    {.id = OSAL_INTERRUPT_INVALID_ID}
};
#elif defined(RCAR_V4M)     /* V4M */
/* PRQA S 1531 1 # It must be external linkage separately from the referenced file because of the object to be exposed to the user. */
const st_osal_interrupt_thread_config_t  g_osal_interrupt_thread_config[] =
{
    /* irq, device_id               interrupt priority            task_name,              stack_size,  interrupt_timeout */
    {579, "ims_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task579",           0x0400,      60000000},
    {580, "ims_01",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task580",           0x0400,      60000000},
    {581, "imr_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task581",           0x0400,      60000000},
    {582, "imr_01",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task582",           0x0400,      60000000},
    {565, "imp_top_00",             OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task565",           0x2000,      60000000},
    {566, "imp_top_00",             OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task566",           0x2000,      60000000},
    {567, "imp_top_00",             OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task567",           0x2000,      60000000},
    {478, "vdsp00_00",              OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task478",           0x2000,      60000000},
    {479, "vdsp00_00",              OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task479",           0x2000,      60000000},
    {480, "vdsp00_00",              OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task480",           0x2000,      60000000},
    {481, "vdsp00_00",              OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task481",           0x2000,      60000000},
    {482, "vdsp00_00",              OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task482",           0x2000,      60000000},
    {483, "vdsp01_00",              OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task483",           0x2000,      60000000},
    {484, "vdsp01_00",              OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task484",           0x2000,      60000000},
    {485, "vdsp01_00",              OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task485",           0x2000,      60000000},
    {486, "vdsp01_00",              OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task486",           0x2000,      60000000},
    {487, "vdsp01_00",              OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task487",           0x2000,      60000000},
    {861, "dof_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task861",           0x0400,      60000000},
    {862, "dof_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task862",           0x0400,      60000000},
    {873, "smd_ps_00",              OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task873",           0x0400,      60000000},
    {874, "smd_ps_00",              OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task874",           0x0400,      60000000},
    {871, "smd_post_00",            OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task871",           0x0400,      60000000},
    {872, "smd_post_00",            OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task872",           0x0400,      60000000},
    {464, "gsx_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task464",           0x1000,      60000000},
    {123, "iccom_00",               OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task123",           0x0400,      60000000},
    {125, "iccom_01",               OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task125",           0x0400,      60000000},
    {127, "iccom_02",               OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task127",           0x0400,      60000000},
    {129, "iccom_03",               OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task129",           0x0400,      60000000},
    {131, "iccom_04",               OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task131",           0x0400,      60000000},
    {133, "iccom_05",               OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task133",           0x0400,      60000000},
    {135, "iccom_06",               OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task135",           0x0400,      60000000},
    {137, "iccom_07",               OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task137",           0x0400,      60000000},
    {916, "iccom_08",               OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task916",           0x0400,      60000000},
    {918, "iccom_09",               OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task918",           0x0400,      60000000},
    {920, "iccom_10",               OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task920",           0x0400,      60000000},
    {922, "iccom_11",               OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task922",           0x0400,      60000000},
    {855, "ivcp1e_00",              OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task855",           0x0400,      60000000},
    {856, "ivcp1e_00",              OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task856",           0x0400,      60000000},
    {475, "cisp_00",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task475",           0x1000,      60000000},
    {473, "tisp_00",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task473",           0x1000,      60000000},
    {474, "tisp_01",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task474",           0x1000,      60000000},
    {556, "vspx_00",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task556",           0x1000,      60000000},
    {852, "fbc_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task852",           0x1000,      60000000},
    {695, "fba_ims0",               OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task695",           0x1000,      60000000},
    {696, "fba_ims1",               OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task696",           0x1000,      60000000},
    {691, "fba_imr0",               OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task691",           0x1000,      60000000},
    {692, "fba_imr1",               OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task692",           0x1000,      60000000},
    {713, "fba_imp0",               OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task713",           0x1000,      60000000},
    {714, "fba_imp1",               OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task714",           0x1000,      60000000},
    {719, "fba_imp_ocv0",           OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task719",           0x1000,      60000000},
    {720, "fba_imp_ocv1",           OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task720",           0x1000,      60000000},
    {721, "fba_imp_ocv2",           OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task721",           0x1000,      60000000},
    {722, "fba_imp_ocv3",           OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task722",           0x1000,      60000000},
    {717, "fba_imp_dp0",            OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task717",           0x1000,      60000000},
    {718, "fba_imp_dp1",            OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task718",           0x1000,      60000000},
    {727, "fba_imp_cnn0",           OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task727",           0x1000,      60000000},
    {738, "fba_imp_dsp0",           OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task738",           0x1000,      60000000},
    {739, "fba_imp_dsp1",           OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task739",           0x1000,      60000000},
    {744, "fba_imp_cnram0",         OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task744",           0x1000,      60000000},
    {701, "fba_umfl0",              OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task701",           0x1000,      60000000},
    {703, "fba_smps0",              OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task703",           0x1000,      60000000},
    {707, "fba_smpo0",              OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task707",           0x1000,      60000000},
    {699, "fba_pap0",               OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task699",           0x1000,      60000000},
    {686, "fba_isp0",               OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task686",           0x1000,      60000000},
    {669, "fba_ca76d0",             OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task669",           0x1000,      60000000},
    {671, "fba_ca760",              OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task671",           0x1000,      60000000},
    {672, "fba_ca761",              OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task672",           0x1000,      60000000},
    {673, "fba_ca762",              OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task673",           0x1000,      60000000},
    {674, "fba_ca763",              OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task674",           0x1000,      60000000},
    {227, "rfso_00",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task227",           0x1000,      60000000},
    {228, "rfso_01",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task228",           0x1000,      60000000},
    {229, "rfso_02",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task229",           0x1000,      60000000},
    {230, "rfso_03",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task230",           0x1000,      60000000},
    {231, "rfso_04",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task231",           0x1000,      60000000},
    {232, "rfso_05",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task232",           0x1000,      60000000},
    {233, "rfso_06",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task233",           0x1000,      60000000},
    {234, "rfso_07",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task234",           0x1000,      60000000},
    {235, "rfso_08",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task235",           0x1000,      60000000},
    {236, "rfso_09",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task236",           0x1000,      60000000},
    {237, "rfso_10",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task237",           0x1000,      60000000},
    {210, "ipmmu_mm_00",            OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task210",           0x1000,      60000000},
    {211, "ipmmu_mm_00",            OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task211",           0x1000,      60000000},
    {900, "caiplite_00",            OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task900",           0x1000,      60000000},
    {902, "caiplite_01",            OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task902",           0x1000,      60000000},
    {904, "caiplite_02",            OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task904",           0x1000,      60000000},
    {906, "caiplite_03",            OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task906",           0x1000,      60000000},
    {908, "caiplite_04",            OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task908",           0x1000,      60000000},
    {910, "caiplite_05",            OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task910",           0x1000,      60000000},
    {912, "caiplite_06",            OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task912",           0x1000,      60000000},
    {914, "caiplite_07",            OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task914",           0x1000,      60000000},
    { 10, "rtdmac_control0_00",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task010",           0x0400,      60000000},
    { 11, "rtdmac_control0_01",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task011",           0x0400,      60000000},
    { 12, "rtdmac_control0_02",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task012",           0x0400,      60000000},
    { 13, "rtdmac_control0_03",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task013",           0x0400,      60000000},
    { 14, "rtdmac_control0_04",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task014",           0x0400,      60000000},
    { 15, "rtdmac_control0_05",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task015",           0x0400,      60000000},
    { 16, "rtdmac_control0_06",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task016",           0x0400,      60000000},
    { 17, "rtdmac_control0_07",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task017",           0x0400,      60000000},
    { 18, "rtdmac_control0_08",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task018",           0x0400,      60000000},
    { 19, "rtdmac_control0_09",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task019",           0x0400,      60000000},
    { 20, "rtdmac_control0_10",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task020",           0x0400,      60000000},
    { 21, "rtdmac_control0_11",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task021",           0x0400,      60000000},
    { 22, "rtdmac_control0_12",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task022",           0x0400,      60000000},
    { 23, "rtdmac_control0_13",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task023",           0x0400,      60000000},
    { 24, "rtdmac_control0_14",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task024",           0x0400,      60000000},
    { 25, "rtdmac_control0_15",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task025",           0x0400,      60000000},
    { 28, "rtdmac_control1_00",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task028",           0x0400,      60000000},
    { 29, "rtdmac_control1_01",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task029",           0x0400,      60000000},
    { 30, "rtdmac_control1_02",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task030",           0x0400,      60000000},
    { 31, "rtdmac_control1_03",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task031",           0x0400,      60000000},
    { 32, "rtdmac_control1_04",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task032",           0x0400,      60000000},
    { 33, "rtdmac_control1_05",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task033",           0x0400,      60000000},
    { 34, "rtdmac_control1_06",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task034",           0x0400,      60000000},
    { 35, "rtdmac_control1_07",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task035",           0x0400,      60000000},
    { 36, "rtdmac_control1_08",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task036",           0x0400,      60000000},
    { 37, "rtdmac_control1_09",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task037",           0x0400,      60000000},
    { 38, "rtdmac_control1_10",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task038",           0x0400,      60000000},
    { 39, "rtdmac_control1_11",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task039",           0x0400,      60000000},
    { 40, "rtdmac_control1_12",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task040",           0x0400,      60000000},
    { 41, "rtdmac_control1_13",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task041",           0x0400,      60000000},
    { 42, "rtdmac_control1_14",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task042",           0x0400,      60000000},
    { 43, "rtdmac_control1_15",     OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task043",           0x0400,      60000000},
    {529, "vin_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task529",           0x0400,      60000000},
    {530, "vin_01",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task530",           0x0400,      60000000},
    {531, "vin_02",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task531",           0x0400,      60000000},
    {532, "vin_03",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task532",           0x0400,      60000000},
    {533, "vin_04",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task533",           0x0400,      60000000},
    {534, "vin_05",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task534",           0x0400,      60000000},
    {535, "vin_06",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task535",           0x0400,      60000000},
    {536, "vin_07",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task536",           0x0400,      60000000},
    {537, "vin_10",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task537",           0x0400,      60000000},
    {538, "vin_11",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task538",           0x0400,      60000000},
    {539, "vin_12",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task539",           0x0400,      60000000},
    {540, "vin_13",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task540",           0x0400,      60000000},
    {541, "vin_14",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task541",           0x0400,      60000000},
    {542, "vin_15",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task542",           0x0400,      60000000},
    {543, "vin_16",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task543",           0x0400,      60000000},
    {544, "vin_17",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task544",           0x0400,      60000000},
    {546, "vsp_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task546",           0x0400,      60000000},
    {499, "csi_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task499",           0x0400,      60000000},
    {500, "csi_01",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task500",           0x0400,      60000000},
    {523, "du_00",                  OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task523",           0x0400,      60000000},
    {524, "du_00",                  OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task524",           0x0400,      60000000},
    {502, "dsi_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task502",           0x0400,      60000000},
    {503, "dsi_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task503",           0x0400,      60000000},
    {504, "dsi_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task504",           0x0400,      60000000},
    {505, "dsi_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task505",           0x0400,      60000000},
    {506, "dsi_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task506",           0x0400,      60000000},
    {507, "dsi_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task507",           0x0400,      60000000},
    {508, "dsi_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task508",           0x0400,      60000000},
    {509, "dsi_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task509",           0x0400,      60000000},
    {510, "dsi_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task510",           0x0400,      60000000},
    {310, "wwdt_00",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task310",           0x0400,      60000000},
    {311, "wwdt_00",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task311",           0x0400,      60000000},
    {312, "wwdt_01",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task312",           0x0400,      60000000},
    {313, "wwdt_01",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task313",           0x0400,      60000000},
    {314, "wwdt_02",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task314",           0x0400,      60000000},
    {315, "wwdt_02",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task315",           0x0400,      60000000},
    {316, "wwdt_03",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task316",           0x0400,      60000000},
    {317, "wwdt_03",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task317",           0x0400,      60000000},
    {318, "wwdt_04",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task318",           0x0400,      60000000},
    {319, "wwdt_04",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task319",           0x0400,      60000000},
    {320, "wwdt_05",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task320",           0x0400,      60000000},
    {321, "wwdt_05",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task321",           0x0400,      60000000},
    {322, "wwdt_06",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task322",           0x0400,      60000000},
    {323, "wwdt_06",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task323",           0x0400,      60000000},
    {324, "wwdt_07",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task324",           0x0400,      60000000},
    {325, "wwdt_07",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task325",           0x0400,      60000000},
    {326, "wwdt_08",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task326",           0x0400,      60000000},
    {327, "wwdt_08",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task327",           0x0400,      60000000},
    {328, "wwdt_09",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task328",           0x0400,      60000000},
    {329, "wwdt_09",                OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task329",           0x0400,      60000000},
    {526, "doc_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task526",           0x0400,      60000000},
    {527, "doc_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task527",           0x0400,      60000000},
    {213, "sdmac_00",               OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task213",           0x0400,      60000000},
    {214, "sdmac_00",               OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task214",           0x0400,      60000000},
    {223, "fbs_00",                 OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task223",           0x0400,      60000000},
    {878, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task878",           0x0400,      60000000},
    {879, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task879",           0x0400,      60000000},
    {880, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task880",           0x0400,      60000000},
    {881, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task881",           0x0400,      60000000},
    {882, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task882",           0x0400,      60000000},
    {883, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task883",           0x0400,      60000000},
    {884, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task884",           0x0400,      60000000},
    {885, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task885",           0x0400,      60000000},
    {886, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task886",           0x0400,      60000000},
    {887, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task887",           0x0400,      60000000},
    {888, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task888",           0x0400,      60000000},
    {889, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task889",           0x0400,      60000000},
    {890, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task890",           0x0400,      60000000},
    {891, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task891",           0x0400,      60000000},
    {892, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task892",           0x0400,      60000000},
    {893, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task893",           0x0400,      60000000},
    {894, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task894",           0x0400,      60000000},
    {895, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task895",           0x0400,      60000000},
    {896, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task896",           0x0400,      60000000},
    {897, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_HIGHEST, "int_task897",           0x0400,      60000000},

    {.id = OSAL_INTERRUPT_INVALID_ID}
};
#else
#error  Device is not defined
#endif
#endif
