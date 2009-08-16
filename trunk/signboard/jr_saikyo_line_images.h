#ifndef JR_SAIKYO_LINE_IMAGES_H
#define JR_SAIKYO_LINE_IMAGES_H

const prog_uint8_t PROGMEM jr_saikyo_line_north_kawagoe[] = {
  0	,	0	,	0	,
  0	,	0	,	0	,
  4	,	65	,	40	,
  36	,	225	,	8	,
  36	,	65	,	126	,
  36	,	65	,	10	,
  36	,	241	,	74	,
  36	,	65	,	82	,
  36	,	81	,	50	,
  36	,	81	,	34	,
  36	,	209	,	182	,
  36	,	81	,	170	,
  34	,	105	,	65	,
  2	,	201	,	0	,
  2	,	137	,	255	,
  0	,	0	,	0
};

const prog_uint8_t PROGMEM jr_saikyo_line_north_omiya[] = {
  0	,	0	,	0	,
  0	,	0	,	0	,
  32	,	0	,	2	,
  32	,	240	,	127	,
  254	,	23	,	64	,
  32	,	208	,	95	,
  32	,	64	,	16	,
  32	,	64	,	16	,
  32	,	192	,	31	,
  80	,	0	,	2	,
  144	,	0	,	1	,
  8	,	225	,	63	,
  4	,	34	,	32	,
  2	,	36	,	32	,
  0	,	224	,	63	,
  0	,	0	,	0
};

const prog_uint8_t PROGMEM jr_saikyo_line_north_akabane[] = {
  0	,	0	,	0	,
  0	,	0	,	0	,
  64	,	224	,	251	,
  252	,	7	,	130	,
  64	,	32	,	138	,
  64	,	64	,	146	,
  254	,	143	,	162	,
  32	,	1	,	130	,
  40	,	5	,	195	,
  36	,	133	,	162	,
  34	,	73	,	146	,
  16	,	41	,	138	,
  16	,	1	,	130	,
  8	,	1	,	130	,
  132	,	1	,	195	,
  0	,	0	,	0
};

const prog_uint8_t PROGMEM jr_saikyo_line_north_ikebukuro[] = {
  0	,	0	,	0	,
  0	,	128	,	4	,
  4	,	129	,	36	,
  40	,	65	,	8	,
  32	,	97	,	127	,
  240	,	79	,	8	,
  34	,	73	,	16	,
  36	,	73	,	100	,
  32	,	9	,	4	,
  40	,	233	,	255	,
  40	,	13	,	74	,
  36	,	128	,	41	,
  36	,	112	,	17	,
  34	,	8	,	47	,
  194	,	207	,	192	,
  0	,	0	,	0
};

const prog_uint8_t PROGMEM jr_saikyo_line_north_musashiurawa[] = {
  0	,	0	,	0	,	0	,	0	,
  0	,	0	,	0	,	0	,	0	,
  64	,	32	,	1	,	20	,	6	,
  92	,	249	,	39	,	132	,	1	,
  64	,	32	,	65	,	31	,	121	,
  254	,	249	,	7	,	196	,	79	,
  64	,	72	,	33	,	31	,	73	,
  72	,	72	,	65	,	149	,	73	,
  72	,	248	,	10	,	95	,	75	,
  202	,	136	,	10	,	85	,	73	,
  186	,	248	,	6	,	95	,	73	,
  138	,	72	,	66	,	21	,	73	,
  170	,	249	,	66	,	21	,	73	,
  26	,	9	,	37	,	21	,	121	,
  6	,	5	,	41	,	21	,	9	,
  0	,	0	,	0	,	0	,	0
};

const prog_uint8_t PROGMEM jr_saikyo_line_south_shinkiba[] = {
  0	,	0	,	0	,	0	,	0	,
  8	,	8	,	4	,	136	,	15	,
  190	,	6	,	4	,	136	,	8	,
  148	,	1	,	4	,	136	,	15	,
  148	,	224	,	255	,	190	,	8	,
  190	,	0	,	4	,	136	,	15	,
  136	,	15	,	14	,	8	,	0	,
  136	,	4	,	21	,	232	,	31	,
  190	,	4	,	21	,	136	,	0	,
  136	,	132	,	36	,	200	,	31	,
  156	,	132	,	36	,	88	,	21	,
  170	,	68	,	68	,	172	,	20	,
  202	,	36	,	132	,	66	,	18	,
  72	,	4	,	4	,	0	,	18	,
  40	,	4	,	4	,	128	,	29	,
  0	,	0	,	0	,	0	,	0
};

const prog_uint8_t PROGMEM jr_saikyo_line_south_osaki[] = {
  0	,	0	,	0	,
  32	,	0	,	16	,
  32	,	64	,	124	,
  32	,	64	,	16	,
  254	,	71	,	40	,
  32	,	80	,	197	,
  32	,	80	,	1	,
  32	,	80	,	255	,
  32	,	80	,	65	,
  80	,	80	,	93	,
  144	,	80	,	85	,
  8	,	241	,	85	,
  4	,	2	,	92	,
  2	,	4	,	64	,
  0	,	0	,	96	,
  0	,	0	,	0
};

#ifndef MYIMAGESTRUCT
#define MYIMAGESTRUCT

struct MyImage {
  uint8_t width;
  uint8_t height;
  const prog_uint8_t *data;
};

#endif

const MyImage jr_saikyo_line_images[] = {
  { 
    24, 16, jr_saikyo_line_north_kawagoe        }
  ,
  { 
    24, 16, jr_saikyo_line_north_omiya          }
  ,
  { 
    24, 16, jr_saikyo_line_north_akabane        }
  ,
  { 
    24, 16, jr_saikyo_line_north_ikebukuro      }
  ,
  { 
    40, 16, jr_saikyo_line_north_musashiurawa   }
  ,
  { 
    40, 16, jr_saikyo_line_south_shinkiba       }
  ,
  { 
    24, 16, jr_saikyo_line_south_osaki          }
};

#endif
