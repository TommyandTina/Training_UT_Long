/**
 *  @file     isr.h
 *  @brief    Application Interface header
 *  @author   H.Kaneko
 *  @version  1.50
 *  @date     History
 *  @date     2016/10/14 :  latest update
 *  @note     This implementation is based on HW document
 *  @date     2017/09/15 : added more supported CPU for R-CarV3H
 *  @date     2018/18/04 : Corrected for Functional Safety
 *  @date     2018/13/08 : [Hoang Pham][V3H]Porting for V3H
 *  @date     2020/01/24 : Changed device dependent implementation to be placed in separate files
 *  @date     2020/02/18 : Deleted INTC-Monitor Counter function
 *  @date     2020/02/19 : Added Unintended Interrupt Check function
 *  @date    2020/03/17  Fixed warnings from Coverity
 *  Copyright (C) 2012-2020, Renesas Electronics Corporation, all rights reserved.
 */
#ifndef ISR_H
#define ISR_H
#ifdef __cplusplus
extern "C" {
#endif

#include "rcar-xos/common/drv.h"

/***********************************************************
Macro definitions
***********************************************************/
/* SPI number */
#define drvISR_SPI000   (0U)    /* SPI 0   */
#define drvISR_SPI001   (1U)    /* SPI 1   */
#define drvISR_SPI002   (2U)    /* SPI 2   */
#define drvISR_SPI003   (3U)    /* SPI 3   */
#define drvISR_SPI004   (4U)    /* SPI 4   */
#define drvISR_SPI005   (5U)    /* SPI 5   */
#define drvISR_SPI006   (6U)    /* SPI 6   */
#define drvISR_SPI007   (7U)    /* SPI 7   */
#define drvISR_SPI008   (8U)    /* SPI 8   */
#define drvISR_SPI009   (9U)    /* SPI 9   */
#define drvISR_SPI010   (10U)   /* SPI 10  */
#define drvISR_SPI011   (11U)   /* SPI 11  */
#define drvISR_SPI012   (12U)   /* SPI 12  */
#define drvISR_SPI013   (13U)   /* SPI 13  */
#define drvISR_SPI014   (14U)   /* SPI 14  */
#define drvISR_SPI015   (15U)   /* SPI 15  */
#define drvISR_SPI016   (16U)   /* SPI 16  */
#define drvISR_SPI017   (17U)   /* SPI 17  */
#define drvISR_SPI018   (18U)   /* SPI 18  */
#define drvISR_SPI019   (19U)   /* SPI 19  */
#define drvISR_SPI020   (20U)   /* SPI 20  */
#define drvISR_SPI021   (21U)   /* SPI 21  */
#define drvISR_SPI022   (22U)   /* SPI 22  */
#define drvISR_SPI023   (23U)   /* SPI 23  */
#define drvISR_SPI024   (24U)   /* SPI 24  */
#define drvISR_SPI025   (25U)   /* SPI 25  */
#define drvISR_SPI026   (26U)   /* SPI 26  */
#define drvISR_SPI027   (27U)   /* SPI 27  */
#define drvISR_SPI028   (28U)   /* SPI 28  */
#define drvISR_SPI029   (29U)   /* SPI 29  */
#define drvISR_SPI030   (30U)   /* SPI 30  */
#define drvISR_SPI031   (31U)   /* SPI 31  */
#define drvISR_SPI032   (32U)   /* SPI 32  */
#define drvISR_SPI033   (33U)   /* SPI 33  */
#define drvISR_SPI034   (34U)   /* SPI 34  */
#define drvISR_SPI035   (35U)   /* SPI 35  */
#define drvISR_SPI036   (36U)   /* SPI 36  */
#define drvISR_SPI037   (37U)   /* SPI 37  */
#define drvISR_SPI038   (38U)   /* SPI 38  */
#define drvISR_SPI039   (39U)   /* SPI 39  */
#define drvISR_SPI040   (40U)   /* SPI 40  */
#define drvISR_SPI041   (41U)   /* SPI 41  */
#define drvISR_SPI042   (42U)   /* SPI 42  */
#define drvISR_SPI043   (43U)   /* SPI 43  */
#define drvISR_SPI044   (44U)   /* SPI 44  */
#define drvISR_SPI045   (45U)   /* SPI 45  */
#define drvISR_SPI046   (46U)   /* SPI 46  */
#define drvISR_SPI047   (47U)   /* SPI 47  */
#define drvISR_SPI048   (48U)   /* SPI 48  */
#define drvISR_SPI049   (49U)   /* SPI 49  */
#define drvISR_SPI050   (50U)   /* SPI 50  */
#define drvISR_SPI051   (51U)   /* SPI 51  */
#define drvISR_SPI052   (52U)   /* SPI 52  */
#define drvISR_SPI053   (53U)   /* SPI 53  */
#define drvISR_SPI054   (54U)   /* SPI 54  */
#define drvISR_SPI055   (55U)   /* SPI 55  */
#define drvISR_SPI056   (56U)   /* SPI 56  */
#define drvISR_SPI057   (57U)   /* SPI 57  */
#define drvISR_SPI058   (58U)   /* SPI 58  */
#define drvISR_SPI059   (59U)   /* SPI 59  */
#define drvISR_SPI060   (60U)   /* SPI 60  */
#define drvISR_SPI061   (61U)   /* SPI 61  */
#define drvISR_SPI062   (62U)   /* SPI 62  */
#define drvISR_SPI063   (63U)   /* SPI 63  */
#define drvISR_SPI064   (64U)   /* SPI 64  */
#define drvISR_SPI065   (65U)   /* SPI 65  */
#define drvISR_SPI066   (66U)   /* SPI 66  */
#define drvISR_SPI067   (67U)   /* SPI 67  */
#define drvISR_SPI068   (68U)   /* SPI 68  */
#define drvISR_SPI069   (69U)   /* SPI 69  */
#define drvISR_SPI070   (70U)   /* SPI 70  */
#define drvISR_SPI071   (71U)   /* SPI 71  */
#define drvISR_SPI072   (72U)   /* SPI 72  */
#define drvISR_SPI073   (73U)   /* SPI 73  */
#define drvISR_SPI074   (74U)   /* SPI 74  */
#define drvISR_SPI075   (75U)   /* SPI 75  */
#define drvISR_SPI076   (76U)   /* SPI 76  */
#define drvISR_SPI077   (77U)   /* SPI 77  */
#define drvISR_SPI078   (78U)   /* SPI 78  */
#define drvISR_SPI079   (79U)   /* SPI 79  */
#define drvISR_SPI080   (80U)   /* SPI 80  */
#define drvISR_SPI081   (81U)   /* SPI 81  */
#define drvISR_SPI082   (82U)   /* SPI 82  */
#define drvISR_SPI083   (83U)   /* SPI 83  */
#define drvISR_SPI084   (84U)   /* SPI 84  */
#define drvISR_SPI085   (85U)   /* SPI 85  */
#define drvISR_SPI086   (86U)   /* SPI 86  */
#define drvISR_SPI087   (87U)   /* SPI 87  */
#define drvISR_SPI088   (88U)   /* SPI 88  */
#define drvISR_SPI089   (89U)   /* SPI 89  */
#define drvISR_SPI090   (90U)   /* SPI 90  */
#define drvISR_SPI091   (91U)   /* SPI 91  */
#define drvISR_SPI092   (92U)   /* SPI 92  */
#define drvISR_SPI093   (93U)   /* SPI 93  */
#define drvISR_SPI094   (94U)   /* SPI 94  */
#define drvISR_SPI095   (95U)   /* SPI 95  */
#define drvISR_SPI096   (96U)   /* SPI 96  */
#define drvISR_SPI097   (97U)   /* SPI 97  */
#define drvISR_SPI098   (98U)   /* SPI 98  */
#define drvISR_SPI099   (99U)   /* SPI 99  */
#define drvISR_SPI100   (100U)  /* SPI 100 */
#define drvISR_SPI101   (101U)  /* SPI 101 */
#define drvISR_SPI102   (102U)  /* SPI 102 */
#define drvISR_SPI103   (103U)  /* SPI 103 */
#define drvISR_SPI104   (104U)  /* SPI 104 */
#define drvISR_SPI105   (105U)  /* SPI 105 */
#define drvISR_SPI106   (106U)  /* SPI 106 */
#define drvISR_SPI107   (107U)  /* SPI 107 */
#define drvISR_SPI108   (108U)  /* SPI 108 */
#define drvISR_SPI109   (109U)  /* SPI 109 */
#define drvISR_SPI110   (110U)  /* SPI 110 */
#define drvISR_SPI111   (111U)  /* SPI 111 */
#define drvISR_SPI112   (112U)  /* SPI 112 */
#define drvISR_SPI113   (113U)  /* SPI 113 */
#define drvISR_SPI114   (114U)  /* SPI 114 */
#define drvISR_SPI115   (115U)  /* SPI 115 */
#define drvISR_SPI116   (116U)  /* SPI 116 */
#define drvISR_SPI117   (117U)  /* SPI 117 */
#define drvISR_SPI118   (118U)  /* SPI 118 */
#define drvISR_SPI119   (119U)  /* SPI 119 */
#define drvISR_SPI120   (120U)  /* SPI 120 */
#define drvISR_SPI121   (121U)  /* SPI 121 */
#define drvISR_SPI122   (122U)  /* SPI 122 */
#define drvISR_SPI123   (123U)  /* SPI 123 */
#define drvISR_SPI124   (124U)  /* SPI 124 */
#define drvISR_SPI125   (125U)  /* SPI 125 */
#define drvISR_SPI126   (126U)  /* SPI 126 */
#define drvISR_SPI127   (127U)  /* SPI 127 */
#define drvISR_SPI128   (128U)  /* SPI 128 */
#define drvISR_SPI129   (129U)  /* SPI 129 */
#define drvISR_SPI130   (130U)  /* SPI 130 */
#define drvISR_SPI131   (131U)  /* SPI 131 */
#define drvISR_SPI132   (132U)  /* SPI 132 */
#define drvISR_SPI133   (133U)  /* SPI 133 */
#define drvISR_SPI134   (134U)  /* SPI 134 */
#define drvISR_SPI135   (135U)  /* SPI 135 */
#define drvISR_SPI136   (136U)  /* SPI 136 */
#define drvISR_SPI137   (137U)  /* SPI 137 */
#define drvISR_SPI138   (138U)  /* SPI 138 */
#define drvISR_SPI139   (139U)  /* SPI 139 */
#define drvISR_SPI140   (140U)  /* SPI 140 */
#define drvISR_SPI141   (141U)  /* SPI 141 */
#define drvISR_SPI142   (142U)  /* SPI 142 */
#define drvISR_SPI143   (143U)  /* SPI 143 */
#define drvISR_SPI144   (144U)  /* SPI 144 */
#define drvISR_SPI145   (145U)  /* SPI 145 */
#define drvISR_SPI146   (146U)  /* SPI 146 */
#define drvISR_SPI147   (147U)  /* SPI 147 */
#define drvISR_SPI148   (148U)  /* SPI 148 */
#define drvISR_SPI149   (149U)  /* SPI 149 */
#define drvISR_SPI150   (150U)  /* SPI 150 */
#define drvISR_SPI151   (151U)  /* SPI 151 */
#define drvISR_SPI152   (152U)  /* SPI 152 */
#define drvISR_SPI153   (153U)  /* SPI 153 */
#define drvISR_SPI154   (154U)  /* SPI 154 */
#define drvISR_SPI155   (155U)  /* SPI 155 */
#define drvISR_SPI156   (156U)  /* SPI 156 */
#define drvISR_SPI157   (157U)  /* SPI 157 */
#define drvISR_SPI158   (158U)  /* SPI 158 */
#define drvISR_SPI159   (159U)  /* SPI 159 */
#define drvISR_SPI160   (160U)  /* SPI 160 */
#define drvISR_SPI161   (161U)  /* SPI 161 */
#define drvISR_SPI162   (162U)  /* SPI 162 */
#define drvISR_SPI163   (163U)  /* SPI 163 */
#define drvISR_SPI164   (164U)  /* SPI 164 */
#define drvISR_SPI165   (165U)  /* SPI 165 */
#define drvISR_SPI166   (166U)  /* SPI 166 */
#define drvISR_SPI167   (167U)  /* SPI 167 */
#define drvISR_SPI168   (168U)  /* SPI 168 */
#define drvISR_SPI169   (169U)  /* SPI 169 */
#define drvISR_SPI170   (170U)  /* SPI 170 */
#define drvISR_SPI171   (171U)  /* SPI 171 */
#define drvISR_SPI172   (172U)  /* SPI 172 */
#define drvISR_SPI173   (173U)  /* SPI 173 */
#define drvISR_SPI174   (174U)  /* SPI 174 */
#define drvISR_SPI175   (175U)  /* SPI 175 */
#define drvISR_SPI176   (176U)  /* SPI 176 */
#define drvISR_SPI177   (177U)  /* SPI 177 */
#define drvISR_SPI178   (178U)  /* SPI 178 */
#define drvISR_SPI179   (179U)  /* SPI 179 */
#define drvISR_SPI180   (180U)  /* SPI 180 */
#define drvISR_SPI181   (181U)  /* SPI 181 */
#define drvISR_SPI182   (182U)  /* SPI 182 */
#define drvISR_SPI183   (183U)  /* SPI 183 */
#define drvISR_SPI184   (184U)  /* SPI 184 */
#define drvISR_SPI185   (185U)  /* SPI 185 */
#define drvISR_SPI186   (186U)  /* SPI 186 */
#define drvISR_SPI187   (187U)  /* SPI 187 */
#define drvISR_SPI188   (188U)  /* SPI 188 */
#define drvISR_SPI189   (189U)  /* SPI 189 */
#define drvISR_SPI190   (190U)  /* SPI 190 */
#define drvISR_SPI191   (191U)  /* SPI 191 */
#define drvISR_SPI192   (192U)  /* SPI 192 */
#define drvISR_SPI193   (193U)  /* SPI 193 */
#define drvISR_SPI194   (194U)  /* SPI 194 */
#define drvISR_SPI195   (195U)  /* SPI 195 */
#define drvISR_SPI196   (196U)  /* SPI 196 */
#define drvISR_SPI197   (197U)  /* SPI 197 */
#define drvISR_SPI198   (198U)  /* SPI 198 */
#define drvISR_SPI199   (199U)  /* SPI 199 */
#define drvISR_SPI200   (200U)  /* SPI 200 */
#define drvISR_SPI201   (201U)  /* SPI 201 */
#define drvISR_SPI202   (202U)  /* SPI 202 */
#define drvISR_SPI203   (203U)  /* SPI 203 */
#define drvISR_SPI204   (204U)  /* SPI 204 */
#define drvISR_SPI205   (205U)  /* SPI 205 */
#define drvISR_SPI206   (206U)  /* SPI 206 */
#define drvISR_SPI207   (207U)  /* SPI 207 */
#define drvISR_SPI208   (208U)  /* SPI 208 */
#define drvISR_SPI209   (209U)  /* SPI 209 */
#define drvISR_SPI210   (210U)  /* SPI 210 */
#define drvISR_SPI211   (211U)  /* SPI 211 */
#define drvISR_SPI212   (212U)  /* SPI 212 */
#define drvISR_SPI213   (213U)  /* SPI 213 */
#define drvISR_SPI214   (214U)  /* SPI 214 */
#define drvISR_SPI215   (215U)  /* SPI 215 */
#define drvISR_SPI216   (216U)  /* SPI 216 */
#define drvISR_SPI217   (217U)  /* SPI 217 */
#define drvISR_SPI218   (218U)  /* SPI 218 */
#define drvISR_SPI219   (219U)  /* SPI 219 */
#define drvISR_SPI220   (220U)  /* SPI 220 */
#define drvISR_SPI221   (221U)  /* SPI 221 */
#define drvISR_SPI222   (222U)  /* SPI 222 */
#define drvISR_SPI223   (223U)  /* SPI 223 */
#define drvISR_SPI224   (224U)  /* SPI 224 */
#define drvISR_SPI225   (225U)  /* SPI 225 */
#define drvISR_SPI226   (226U)  /* SPI 226 */
#define drvISR_SPI227   (227U)  /* SPI 227 */
#define drvISR_SPI228   (228U)  /* SPI 228 */
#define drvISR_SPI229   (229U)  /* SPI 229 */
#define drvISR_SPI230   (230U)  /* SPI 230 */
#define drvISR_SPI231   (231U)  /* SPI 231 */
#define drvISR_SPI232   (232U)  /* SPI 232 */
#define drvISR_SPI233   (233U)  /* SPI 233 */
#define drvISR_SPI234   (234U)  /* SPI 234 */
#define drvISR_SPI235   (235U)  /* SPI 235 */
#define drvISR_SPI236   (236U)  /* SPI 236 */
#define drvISR_SPI237   (237U)  /* SPI 237 */
#define drvISR_SPI238   (238U)  /* SPI 238 */
#define drvISR_SPI239   (239U)  /* SPI 239 */
#define drvISR_SPI240   (240U)  /* SPI 240 */
#define drvISR_SPI241   (241U)  /* SPI 241 */
#define drvISR_SPI242   (242U)  /* SPI 242 */
#define drvISR_SPI243   (243U)  /* SPI 243 */
#define drvISR_SPI244   (244U)  /* SPI 244 */
#define drvISR_SPI245   (245U)  /* SPI 245 */
#define drvISR_SPI246   (246U)  /* SPI 246 */
#define drvISR_SPI247   (247U)  /* SPI 247 */
#define drvISR_SPI248   (248U)  /* SPI 248 */
#define drvISR_SPI249   (249U)  /* SPI 249 */
#define drvISR_SPI250   (250U)  /* SPI 250 */
#define drvISR_SPI251   (251U)  /* SPI 251 */
#define drvISR_SPI252   (252U)  /* SPI 252 */
#define drvISR_SPI253   (253U)  /* SPI 253 */
#define drvISR_SPI254   (254U)  /* SPI 254 */
#define drvISR_SPI255   (255U)  /* SPI 255 */
#define drvISR_SPI256   (256U)  /* SPI 256 */
#define drvISR_SPI257   (257U)  /* SPI 257 */
#define drvISR_SPI258   (258U)  /* SPI 258 */
#define drvISR_SPI259   (259U)  /* SPI 259 */
#define drvISR_SPI260   (260U)  /* SPI 260 */
#define drvISR_SPI261   (261U)  /* SPI 261 */
#define drvISR_SPI262   (262U)  /* SPI 262 */
#define drvISR_SPI263   (263U)  /* SPI 263 */
#define drvISR_SPI264   (264U)  /* SPI 264 */
#define drvISR_SPI265   (265U)  /* SPI 265 */
#define drvISR_SPI266   (266U)  /* SPI 266 */
#define drvISR_SPI267   (267U)  /* SPI 267 */
#define drvISR_SPI268   (268U)  /* SPI 268 */
#define drvISR_SPI269   (269U)  /* SPI 269 */
#define drvISR_SPI270   (270U)  /* SPI 270 */
#define drvISR_SPI271   (271U)  /* SPI 271 */
#define drvISR_SPI272   (272U)  /* SPI 272 */
#define drvISR_SPI273   (273U)  /* SPI 273 */
#define drvISR_SPI274   (274U)  /* SPI 274 */
#define drvISR_SPI275   (275U)  /* SPI 275 */
#define drvISR_SPI276   (276U)  /* SPI 276 */
#define drvISR_SPI277   (277U)  /* SPI 277 */
#define drvISR_SPI278   (278U)  /* SPI 278 */
#define drvISR_SPI279   (279U)  /* SPI 279 */
#define drvISR_SPI280   (280U)  /* SPI 280 */
#define drvISR_SPI281   (281U)  /* SPI 281 */
#define drvISR_SPI282   (282U)  /* SPI 282 */
#define drvISR_SPI283   (283U)  /* SPI 283 */
#define drvISR_SPI284   (284U)  /* SPI 284 */
#define drvISR_SPI285   (285U)  /* SPI 285 */
#define drvISR_SPI286   (286U)  /* SPI 286 */
#define drvISR_SPI287   (287U)  /* SPI 287 */
#define drvISR_SPI288   (288U)  /* SPI 288 */
#define drvISR_SPI289   (289U)  /* SPI 289 */
#define drvISR_SPI290   (290U)  /* SPI 290 */
#define drvISR_SPI291   (291U)  /* SPI 291 */
#define drvISR_SPI292   (292U)  /* SPI 292 */
#define drvISR_SPI293   (293U)  /* SPI 293 */
#define drvISR_SPI294   (294U)  /* SPI 294 */
#define drvISR_SPI295   (295U)  /* SPI 295 */
#define drvISR_SPI296   (296U)  /* SPI 296 */
#define drvISR_SPI297   (297U)  /* SPI 297 */
#define drvISR_SPI298   (298U)  /* SPI 298 */
#define drvISR_SPI299   (299U)  /* SPI 299 */
#define drvISR_SPI300   (300U)  /* SPI 300 */
#define drvISR_SPI301   (301U)  /* SPI 301 */
#define drvISR_SPI302   (302U)  /* SPI 302 */
#define drvISR_SPI303   (303U)  /* SPI 303 */
#define drvISR_SPI304   (304U)  /* SPI 304 */
#define drvISR_SPI305   (305U)  /* SPI 305 */
#define drvISR_SPI306   (306U)  /* SPI 306 */
#define drvISR_SPI307   (307U)  /* SPI 307 */
#define drvISR_SPI308   (308U)  /* SPI 308 */
#define drvISR_SPI309   (309U)  /* SPI 309 */
#define drvISR_SPI310   (310U)  /* SPI 310 */
#define drvISR_SPI311   (311U)  /* SPI 311 */
#define drvISR_SPI312   (312U)  /* SPI 312 */
#define drvISR_SPI313   (313U)  /* SPI 313 */
#define drvISR_SPI314   (314U)  /* SPI 314 */
#define drvISR_SPI315   (315U)  /* SPI 315 */
#define drvISR_SPI316   (316U)  /* SPI 316 */
#define drvISR_SPI317   (317U)  /* SPI 317 */
#define drvISR_SPI318   (318U)  /* SPI 318 */
#define drvISR_SPI319   (319U)  /* SPI 319 */
#define drvISR_SPI320   (320U)  /* SPI 320 */
#define drvISR_SPI321   (321U)  /* SPI 321 */
#define drvISR_SPI322   (322U)  /* SPI 322 */
#define drvISR_SPI323   (323U)  /* SPI 323 */
#define drvISR_SPI324   (324U)  /* SPI 324 */
#define drvISR_SPI325   (325U)  /* SPI 325 */
#define drvISR_SPI326   (326U)  /* SPI 326 */
#define drvISR_SPI327   (327U)  /* SPI 327 */
#define drvISR_SPI328   (328U)  /* SPI 328 */
#define drvISR_SPI329   (329U)  /* SPI 329 */
#define drvISR_SPI330   (330U)  /* SPI 330 */
#define drvISR_SPI331   (331U)  /* SPI 331 */
#define drvISR_SPI332   (332U)  /* SPI 332 */
#define drvISR_SPI333   (333U)  /* SPI 333 */
#define drvISR_SPI334   (334U)  /* SPI 334 */
#define drvISR_SPI335   (335U)  /* SPI 335 */
#define drvISR_SPI336   (336U)  /* SPI 336 */
#define drvISR_SPI337   (337U)  /* SPI 337 */
#define drvISR_SPI338   (338U)  /* SPI 338 */
#define drvISR_SPI339   (339U)  /* SPI 339 */
#define drvISR_SPI340   (340U)  /* SPI 340 */
#define drvISR_SPI341   (341U)  /* SPI 341 */
#define drvISR_SPI342   (342U)  /* SPI 342 */
#define drvISR_SPI343   (343U)  /* SPI 343 */
#define drvISR_SPI344   (344U)  /* SPI 344 */
#define drvISR_SPI345   (345U)  /* SPI 345 */
#define drvISR_SPI346   (346U)  /* SPI 346 */
#define drvISR_SPI347   (347U)  /* SPI 347 */
#define drvISR_SPI348   (348U)  /* SPI 348 */
#define drvISR_SPI349   (349U)  /* SPI 349 */
#define drvISR_SPI350   (350U)  /* SPI 350 */
#define drvISR_SPI351   (351U)  /* SPI 351 */
#define drvISR_SPI352   (352U)  /* SPI 352 */
#define drvISR_SPI353   (353U)  /* SPI 353 */
#define drvISR_SPI354   (354U)  /* SPI 354 */
#define drvISR_SPI355   (355U)  /* SPI 355 */
#define drvISR_SPI356   (356U)  /* SPI 356 */
#define drvISR_SPI357   (357U)  /* SPI 357 */
#define drvISR_SPI358   (358U)  /* SPI 358 */
#define drvISR_SPI359   (359U)  /* SPI 359 */
#define drvISR_SPI360   (360U)  /* SPI 360 */
#define drvISR_SPI361   (361U)  /* SPI 361 */
#define drvISR_SPI362   (362U)  /* SPI 362 */
#define drvISR_SPI363   (363U)  /* SPI 363 */
#define drvISR_SPI364   (364U)  /* SPI 364 */
#define drvISR_SPI365   (365U)  /* SPI 365 */
#define drvISR_SPI366   (366U)  /* SPI 366 */
#define drvISR_SPI367   (367U)  /* SPI 367 */
#define drvISR_SPI368   (368U)  /* SPI 368 */
#define drvISR_SPI369   (369U)  /* SPI 369 */
#define drvISR_SPI370   (370U)  /* SPI 370 */
#define drvISR_SPI371   (371U)  /* SPI 371 */
#define drvISR_SPI372   (372U)  /* SPI 372 */
#define drvISR_SPI373   (373U)  /* SPI 373 */
#define drvISR_SPI374   (374U)  /* SPI 374 */
#define drvISR_SPI375   (375U)  /* SPI 375 */
#define drvISR_SPI376   (376U)  /* SPI 376 */
#define drvISR_SPI377   (377U)  /* SPI 377 */
#define drvISR_SPI378   (378U)  /* SPI 378 */
#define drvISR_SPI379   (379U)  /* SPI 379 */
#define drvISR_SPI380   (380U)  /* SPI 380 */
#define drvISR_SPI381   (381U)  /* SPI 381 */
#define drvISR_SPI382   (382U)  /* SPI 382 */
#define drvISR_SPI383   (383U)  /* SPI 383 */
#define drvISR_SPI384   (384U)  /* SPI 384 */
#define drvISR_SPI385   (385U)  /* SPI 385 */
#define drvISR_SPI386   (386U)  /* SPI 386 */
#define drvISR_SPI387   (387U)  /* SPI 387 */
#define drvISR_SPI388   (388U)  /* SPI 388 */
#define drvISR_SPI389   (389U)  /* SPI 389 */
#define drvISR_SPI390   (390U)  /* SPI 390 */
#define drvISR_SPI391   (391U)  /* SPI 391 */
#define drvISR_SPI392   (392U)  /* SPI 392 */
#define drvISR_SPI393   (393U)  /* SPI 393 */
#define drvISR_SPI394   (394U)  /* SPI 394 */
#define drvISR_SPI395   (395U)  /* SPI 395 */
#define drvISR_SPI396   (396U)  /* SPI 396 */
#define drvISR_SPI397   (397U)  /* SPI 397 */
#define drvISR_SPI398   (398U)  /* SPI 398 */
#define drvISR_SPI399   (399U)  /* SPI 399 */
#define drvISR_SPI400   (400U)  /* SPI 400 */
#define drvISR_SPI401   (401U)  /* SPI 401 */
#define drvISR_SPI402   (402U)  /* SPI 402 */
#define drvISR_SPI403   (403U)  /* SPI 403 */
#define drvISR_SPI404   (404U)  /* SPI 404 */
#define drvISR_SPI405   (405U)  /* SPI 405 */
#define drvISR_SPI406   (406U)  /* SPI 406 */
#define drvISR_SPI407   (407U)  /* SPI 407 */
#define drvISR_SPI408   (408U)  /* SPI 408 */
#define drvISR_SPI409   (409U)  /* SPI 409 */
#define drvISR_SPI410   (410U)  /* SPI 410 */
#define drvISR_SPI411   (411U)  /* SPI 411 */
#define drvISR_SPI412   (412U)  /* SPI 412 */
#define drvISR_SPI413   (413U)  /* SPI 413 */
#define drvISR_SPI414   (414U)  /* SPI 414 */
#define drvISR_SPI415   (415U)  /* SPI 415 */
#define drvISR_SPI416   (416U)  /* SPI 416 */
#define drvISR_SPI417   (417U)  /* SPI 417 */
#define drvISR_SPI418   (418U)  /* SPI 418 */
#define drvISR_SPI419   (419U)  /* SPI 419 */
#define drvISR_SPI420   (420U)  /* SPI 420 */
#define drvISR_SPI421   (421U)  /* SPI 421 */
#define drvISR_SPI422   (422U)  /* SPI 422 */
#define drvISR_SPI423   (423U)  /* SPI 423 */
#define drvISR_SPI424   (424U)  /* SPI 424 */
#define drvISR_SPI425   (425U)  /* SPI 425 */
#define drvISR_SPI426   (426U)  /* SPI 426 */
#define drvISR_SPI427   (427U)  /* SPI 427 */
#define drvISR_SPI428   (428U)  /* SPI 428 */
#define drvISR_SPI429   (429U)  /* SPI 429 */
#define drvISR_SPI430   (430U)  /* SPI 430 */
#define drvISR_SPI431   (431U)  /* SPI 431 */
#define drvISR_SPI432   (432U)  /* SPI 432 */
#define drvISR_SPI433   (433U)  /* SPI 433 */
#define drvISR_SPI434   (434U)  /* SPI 434 */
#define drvISR_SPI435   (435U)  /* SPI 435 */
#define drvISR_SPI436   (436U)  /* SPI 436 */
#define drvISR_SPI437   (437U)  /* SPI 437 */
#define drvISR_SPI438   (438U)  /* SPI 438 */
#define drvISR_SPI439   (439U)  /* SPI 439 */
#define drvISR_SPI440   (440U)  /* SPI 440 */
#define drvISR_SPI441   (441U)  /* SPI 441 */
#define drvISR_SPI442   (442U)  /* SPI 442 */
#define drvISR_SPI443   (443U)  /* SPI 443 */
#define drvISR_SPI444   (444U)  /* SPI 444 */
#define drvISR_SPI445   (445U)  /* SPI 445 */
#define drvISR_SPI446   (446U)  /* SPI 446 */
#define drvISR_SPI447   (447U)  /* SPI 447 */
#define drvISR_SPI448   (448U)  /* SPI 448 */
#define drvISR_SPI449   (449U)  /* SPI 449 */
#define drvISR_SPI450   (450U)  /* SPI 450 */
#define drvISR_SPI451   (451U)  /* SPI 451 */
#define drvISR_SPI452   (452U)  /* SPI 452 */
#define drvISR_SPI453   (453U)  /* SPI 453 */
#define drvISR_SPI454   (454U)  /* SPI 454 */
#define drvISR_SPI455   (455U)  /* SPI 455 */
#define drvISR_SPI456   (456U)  /* SPI 456 */
#define drvISR_SPI457   (457U)  /* SPI 457 */
#define drvISR_SPI458   (458U)  /* SPI 458 */
#define drvISR_SPI459   (459U)  /* SPI 459 */
#define drvISR_SPI460   (460U)  /* SPI 460 */
#define drvISR_SPI461   (461U)  /* SPI 461 */
#define drvISR_SPI462   (462U)  /* SPI 462 */
#define drvISR_SPI463   (463U)  /* SPI 463 */
#define drvISR_SPI464   (464U)  /* SPI 464 */
#define drvISR_SPI465   (465U)  /* SPI 465 */
#define drvISR_SPI466   (466U)  /* SPI 466 */
#define drvISR_SPI467   (467U)  /* SPI 467 */
#define drvISR_SPI468   (468U)  /* SPI 468 */
#define drvISR_SPI469   (469U)  /* SPI 469 */
#define drvISR_SPI470   (470U)  /* SPI 470 */
#define drvISR_SPI471   (471U)  /* SPI 471 */
#define drvISR_SPI472   (472U)  /* SPI 472 */
#define drvISR_SPI473   (473U)  /* SPI 473 */
#define drvISR_SPI474   (474U)  /* SPI 474 */
#define drvISR_SPI475   (475U)  /* SPI 475 */
#define drvISR_SPI476   (476U)  /* SPI 476 */
#define drvISR_SPI477   (477U)  /* SPI 477 */
#define drvISR_SPI478   (478U)  /* SPI 478 */
#define drvISR_SPI479   (479U)  /* SPI 479 */

/* CPU Target */
#define drvISR_CPU_0    (0U)    /* CPU 0 */
#define drvISR_CPU_1    (1U)    /* CPU 1 */

/* Priority */
#define drvISR_PRIORITY_0   (0U)    /* 0  */
#define drvISR_PRIORITY_1   (1U)    /* 1  */
#define drvISR_PRIORITY_2   (2U)    /* 2  */
#define drvISR_PRIORITY_3   (3U)    /* 3  */
#define drvISR_PRIORITY_4   (4U)    /* 4  */
#define drvISR_PRIORITY_5   (5U)    /* 5  */
#define drvISR_PRIORITY_6   (6U)    /* 6  */
#define drvISR_PRIORITY_7   (7U)    /* 7  */
#define drvISR_PRIORITY_8   (8U)    /* 8  */
#define drvISR_PRIORITY_9   (9U)    /* 9  */
#define drvISR_PRIORITY_10  (10U)   /* 10 */
#define drvISR_PRIORITY_11  (11U)   /* 11 */
#define drvISR_PRIORITY_12  (12U)   /* 12 */
#define drvISR_PRIORITY_13  (13U)   /* 13 */
#define drvISR_PRIORITY_14  (14U)   /* 14 */

/***********************************************************
Typedef definitions
***********************************************************/
/* Type definition of the callback function */
typedef void (*drvISR_CbFunc_t)(UINT16 SpiID);
typedef void (*drvISR_FaultCbFunc_t)(UINT16 SpiID);

typedef enum
{
    drvISR_LEVEL_SENSITIVE,
    drvISR_EDGE_TRIGGERED
} drvISR_InterruptType_t;

/***********************************************************
Exported global variables
***********************************************************/

/***********************************************************
Exported global functions (to be accessed by other files)
***********************************************************/

/* API of the ISR driver */
static inline UINT16 drvISR_SetupSPI(UINT16 SpiID, UINT16 TargetCPU, UINT16 Priority, drvISR_CbFunc_t CallbackFunc)
{
	return drv_ERR_BUS_INTERFACE_FAULT;
}

static inline UINT16 drvISR_RemoveSPI(UINT16 SpiID)
{
	return drv_ERR_BUS_INTERFACE_FAULT;
}

static inline UINT16 drvISR_EnableSPI(UINT16 SpiID)
{
	return drv_ERR_BUS_INTERFACE_FAULT;
}

static inline UINT16 drvISR_DisableSPI(UINT16 SpiID)
{
	return drv_ERR_BUS_INTERFACE_FAULT;
}

static inline UINT16 drvISR_SetIntTypeSPI(UINT16 SpiID, drvISR_InterruptType_t IntType)
{
	return drv_ERR_BUS_INTERFACE_FAULT;
}

/* IRQ interrupt handler */
static inline UINT16 drvISR_InterruptHandler(UINT32 InterruptID)
{
	return drv_ERR_BUS_INTERFACE_FAULT;
}

/********************************************************/
/* Configuration Register Check (for Functional Safety) */
/********************************************************/
static inline UINT16 drvISR_CheckConfigReg(void)
{
	return drv_ERR_BUS_INTERFACE_FAULT;
}

/******************************************************/
/* Unintended Interrupt Check (for Functional Safety) */
/******************************************************/
static inline UINT16 drvISR_SetFaultCallback(drvISR_FaultCbFunc_t CallbackFunc)
{
	return drv_ERR_BUS_INTERFACE_FAULT;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif  /* ISR_H */
