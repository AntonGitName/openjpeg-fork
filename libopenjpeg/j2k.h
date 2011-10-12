/*
 * Copyright (c) 2002-2007, Communications and Remote Sensing Laboratory, Universite catholique de Louvain (UCL), Belgium
 * Copyright (c) 2002-2007, Professor Benoit Macq
 * Copyright (c) 2001-2003, David Janssens
 * Copyright (c) 2002-2003, Yannick Verschueren
 * Copyright (c) 2003-2007, Francois-Olivier Devaux and Antonin Descampe
 * Copyright (c) 2005, Herve Drolon, FreeImage Team
 * Copyright (c) 2006-2007, Parvatha Elangovan
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS `AS IS'
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef __J2K_H
#define __J2K_H
/**
@file j2k.h
@brief The JPEG-2000 Codestream Reader/Writer (J2K)

The functions in J2K.C have for goal to read/write the several parts of the codestream: markers and data.
*/

/** @defgroup J2K J2K - JPEG-2000 codestream reader/writer */
/*@{*/

#define J2K_CP_CSTY_PRT 0x01
#define J2K_CP_CSTY_SOP 0x02
#define J2K_CP_CSTY_EPH 0x04
#define J2K_CCP_CSTY_PRT 0x01
#define J2K_CCP_CBLKSTY_LAZY 0x01     /**< Selective arithmetic coding bypass */
#define J2K_CCP_CBLKSTY_RESET 0x02    /**< Reset context probabilities on coding pass boundaries */
#define J2K_CCP_CBLKSTY_TERMALL 0x04  /**< Termination on each coding pass */
#define J2K_CCP_CBLKSTY_VSC 0x08      /**< Vertically stripe causal context */
#define J2K_CCP_CBLKSTY_PTERM 0x10    /**< Predictable termination */
#define J2K_CCP_CBLKSTY_SEGSYM 0x20   /**< Segmentation symbols are used */
#define J2K_CCP_QNTSTY_NOQNT 0
#define J2K_CCP_QNTSTY_SIQNT 1
#define J2K_CCP_QNTSTY_SEQNT 2

/* ----------------------------------------------------------------------- */

#define J2K_MS_SOC 0xff4f	/**< SOC marker value */
#define J2K_MS_SOT 0xff90	/**< SOT marker value */
#define J2K_MS_SOD 0xff93	/**< SOD marker value */
#define J2K_MS_EOC 0xffd9	/**< EOC marker value */
#define J2K_MS_SIZ 0xff51	/**< SIZ marker value */
#define J2K_MS_COD 0xff52	/**< COD marker value */
#define J2K_MS_COC 0xff53	/**< COC marker value */
#define J2K_MS_RGN 0xff5e	/**< RGN marker value */
#define J2K_MS_QCD 0xff5c	/**< QCD marker value */
#define J2K_MS_QCC 0xff5d	/**< QCC marker value */
#define J2K_MS_POC 0xff5f	/**< POC marker value */
#define J2K_MS_TLM 0xff55	/**< TLM marker value */
#define J2K_MS_PLM 0xff57	/**< PLM marker value */
#define J2K_MS_PLT 0xff58	/**< PLT marker value */
#define J2K_MS_PPM 0xff60	/**< PPM marker value */
#define J2K_MS_PPT 0xff61	/**< PPT marker value */
#define J2K_MS_SOP 0xff91	/**< SOP marker value */
#define J2K_MS_EPH 0xff92	/**< EPH marker value */
#define J2K_MS_CRG 0xff63	/**< CRG marker value */
#define J2K_MS_COM 0xff64	/**< COM marker value */
#define J2K_MS_CBD 0xff78	/**< CBD marker value */
#define J2K_MS_MCC 0xff75	/**< MCC marker value */
#define J2K_MS_MCT 0xff74	/**< MCT marker value */
#define J2K_MS_MCO 0xff77	/**< MCO marker value */

#define J2K_MS_UNK 0		/**< UNKNOWN marker value */

/* UniPG>> */
#ifdef USE_JPWL
#define J2K_MS_EPC 0xff68	/**< EPC marker value (Part 11: JPEG 2000 for Wireless) */
#define J2K_MS_EPB 0xff66	/**< EPB marker value (Part 11: JPEG 2000 for Wireless) */ 
#define J2K_MS_ESD 0xff67	/**< ESD marker value (Part 11: JPEG 2000 for Wireless) */ 
#define J2K_MS_RED 0xff69	/**< RED marker value (Part 11: JPEG 2000 for Wireless) */
#endif /* USE_JPWL */
#ifdef USE_JPSEC
#define J2K_MS_SEC 0xff65    /**< SEC marker value (Part 8: Secure JPEG 2000) */
#define J2K_MS_INSEC 0xff94  /**< INSEC marker value (Part 8: Secure JPEG 2000) */
#endif /* USE_JPSEC */
/* <<UniPG */


struct opj_stream_private;

/* ----------------------------------------------------------------------- */

/**
Values that specify the status of the decoding process when decoding the main header. 
These values may be combined with a | operator. 
*/
typedef enum J2K_STATUS {
	J2K_STATE_MHSOC  = 0x0001, /**< a SOC marker is expected */
	J2K_STATE_MHSIZ  = 0x0002, /**< a SIZ marker is expected */
	J2K_STATE_MH     = 0x0004, /**< the decoding process is in the main header */
	J2K_STATE_TPHSOT = 0x0008, /**< the decoding process is in a tile part header and expects a SOT marker */
	J2K_STATE_TPH    = 0x0010, /**< the decoding process is in a tile part header */
	J2K_STATE_MT     = 0x0020, /**< the EOC marker has just been read */
	J2K_STATE_NEOC   = 0x0040, /**< the decoding process must not expect a EOC marker because the codestream is truncated */

	J2K_STATE_EOC	 = 0x0100,  /**< the decoding process has encountered the EOC marker */
	J2K_STATE_ERR    = 0x8000 /**< the decoding process has encountered an error (FIXME warning V1 = 0x0080)*/
} J2K_STATUS;

/**
 * Type of elements storing in the MCT data
 */
typedef enum MCT_ELEMENT_TYPE
{
	MCT_TYPE_INT16 = 0,		/** MCT data is stored as signed shorts*/
	MCT_TYPE_INT32 = 1,		/** MCT data is stored as signed integers*/
	MCT_TYPE_FLOAT = 2,		/** MCT data is stored as floats*/
	MCT_TYPE_DOUBLE = 3		/** MCT data is stored as doubles*/
} J2K_MCT_ELEMENT_TYPE;

/**
 * Type of MCT array
 */
typedef enum MCT_ARRAY_TYPE
{
	MCT_TYPE_DEPENDENCY = 0,
	MCT_TYPE_DECORRELATION = 1,
	MCT_TYPE_OFFSET = 2
} J2K_MCT_ARRAY_TYPE;

/* ----------------------------------------------------------------------- */

/** 
T2 encoding mode 
*/
typedef enum T2_MODE {
	THRESH_CALC = 0,	/** Function called in Rate allocation process*/
	FINAL_PASS = 1		/** Function called in Tier 2 process*/
}J2K_T2_MODE;

/**
Quantization stepsize
*/
typedef struct opj_stepsize {
	/** exponent */
	int expn;
	/** mantissa */
	int mant;
} opj_stepsize_t;

//OPJ_V1/**
//Tile-component coding parameters
//*/
//typedef struct opj_tccp {
//	/** coding style */
//	int csty;
//	/** number of resolutions */
//	int numresolutions;
//	/** code-blocks width */
//	int cblkw;
//	/** code-blocks height */
//	int cblkh;
//	/** code-block coding style */
//	int cblksty;
//	/** discrete wavelet transform identifier */
//	int qmfbid;
//	/** quantisation style */
//	int qntsty;
//	/** stepsizes used for quantization */
//	opj_stepsize_t stepsizes[J2K_MAXBANDS];
//	/** number of guard bits */
//	int numgbits;
//	/** Region Of Interest shift */
//	int roishift;
//	/** precinct width */
//	int prcw[J2K_MAXRLVLS];
//	/** precinct height */
//	int prch[J2K_MAXRLVLS];
//} opj_tccp_t;

/**
Tile-component coding parameters
*/
typedef struct opj_tccp
{
	/** coding style */
	OPJ_UINT32 csty;
	/** number of resolutions */
	OPJ_UINT32 numresolutions;
	/** code-blocks width */
	OPJ_UINT32 cblkw;
	/** code-blocks height */
	OPJ_UINT32 cblkh;
	/** code-block coding style */
	OPJ_UINT32 cblksty;
	/** discrete wavelet transform identifier */
	OPJ_UINT32 qmfbid;
	/** quantisation style */
	OPJ_UINT32 qntsty;
	/** stepsizes used for quantization */
	opj_stepsize_t stepsizes[J2K_MAXBANDS];
	/** number of guard bits */
	OPJ_UINT32 numgbits;
	/** Region Of Interest shift */
	OPJ_INT32 roishift;
	/** precinct width */
	OPJ_UINT32 prcw[J2K_MAXRLVLS];
	/** precinct height */
	OPJ_UINT32 prch[J2K_MAXRLVLS];
	/** the dc_level_shift **/
	OPJ_INT32 m_dc_level_shift;
}
opj_tccp_t;


/**
Tile coding parameters : 
this structure is used to store coding/decoding parameters common to all
tiles (information like COD, COC in main header)
*/
typedef struct opj_tcp {
	/** 1 : first part-tile of a tile */
	int first;
	/** coding style */
	int csty;
	/** progression order */
	OPJ_PROG_ORDER prg;
	/** number of layers */
	int numlayers;
	/** multi-component transform identifier */
	int mct;
	/** rates of layers */
	float rates[100];
	/** number of progression order changes */
	int numpocs;
	/** indicates if a POC marker has been used O:NO, 1:YES */
	int POC;
	/** progression order changes */
	opj_poc_t pocs[32];
	/** packet header store there for futur use in t2_decode_packet */
	unsigned char *ppt_data;
	/** pointer remaining on the first byte of the first header if ppt is used */
	unsigned char *ppt_data_first;
	/** If ppt == 1 --> there was a PPT marker for the present tile */
	int ppt;
	/** used in case of multiple marker PPT (number of info already stored) */
	int ppt_store;
	/** ppmbug1 */
	int ppt_len;
	/** add fixed_quality */
	float distoratio[100];
	/** tile-component coding parameters */
	opj_tccp_t *tccps;
} opj_tcp_t;

/**
 * FIXME DOC
 */
typedef struct opj_mct_data
{
	J2K_MCT_ELEMENT_TYPE m_element_type;
	J2K_MCT_ARRAY_TYPE	 m_array_type;
	OPJ_UINT32			 m_index;
	OPJ_BYTE *			 m_data;
	OPJ_UINT32			 m_data_size;
}
opj_mct_data_t;

/**
 * FIXME DOC
 */
typedef struct opj_simple_mcc_decorrelation_data
{
	OPJ_UINT32			 m_index;
	OPJ_UINT32			 m_nb_comps;
	opj_mct_data_t *	 m_decorrelation_array;
	opj_mct_data_t *	 m_offset_array;
	OPJ_UINT32			 m_is_irreversible : 1;
}
opj_simple_mcc_decorrelation_data_t;

/**
Tile coding parameters :
this structure is used to store coding/decoding parameters common to all
tiles (information like COD, COC in main header)
*/
typedef struct opj_tcp_v2
{
	/** coding style */
	OPJ_UINT32 csty;
	/** progression order */
	OPJ_PROG_ORDER prg;
	/** number of layers */
	OPJ_UINT32 numlayers;
	OPJ_UINT32 num_layers_to_decode;
	/** multi-component transform identifier */
	OPJ_UINT32 mct;
	/** rates of layers */
	OPJ_FLOAT32 rates[100];
	/** number of progression order changes */
	OPJ_UINT32 numpocs;
	/** progression order changes */
	opj_poc_t pocs[32];
	/** packet header store there for futur use in t2_decode_packet */
	OPJ_BYTE *ppt_data;
	/** used to keep a track of the allocated memory */
	OPJ_BYTE *ppt_buffer;
	/** Number of bytes stored inside ppt_data*/
	OPJ_UINT32 ppt_data_size;
	/** size of ppt_data*/
	OPJ_UINT32 ppt_len;
	/** add fixed_quality */
	OPJ_FLOAT32 distoratio[100];
	/** tile-component coding parameters */
	opj_tccp_t *tccps;
	/** number of tile parts for the tile. */
	OPJ_UINT32 m_nb_tile_parts;
	/** data for the tile */
	OPJ_BYTE *		m_data;
	/** size of data */
	OPJ_UINT32		m_data_size;
	/** encoding norms */
	OPJ_FLOAT64 *	mct_norms;
	/** the mct decoding matrix */
	OPJ_FLOAT32 *	m_mct_decoding_matrix;
	/** the mct coding matrix */
	OPJ_FLOAT32 *	m_mct_coding_matrix;
	/** mct records */
	opj_mct_data_t * m_mct_records;
	/** the number of mct records. */
	OPJ_UINT32 m_nb_mct_records;
	/** the max number of mct records. */
	OPJ_UINT32 m_nb_max_mct_records;
	/** mcc records */
	opj_simple_mcc_decorrelation_data_t * m_mcc_records;
	/** the number of mct records. */
	OPJ_UINT32 m_nb_mcc_records;
	/** the max number of mct records. */
	OPJ_UINT32 m_nb_max_mcc_records;



	/***** FLAGS *******/
	/** If ppt == 1 --> there was a PPT marker for the present tile */
	OPJ_UINT32 ppt : 1;
	/** indicates if a POC marker has been used O:NO, 1:YES */
	OPJ_UINT32 POC : 1;
} opj_tcp_v2_t;





/**
Coding parameters
*/
typedef struct opj_cp {
	/** Digital cinema profile*/
	OPJ_CINEMA_MODE cinema;
	/** Maximum rate for each component. If == 0, component size limitation is not considered */
	int max_comp_size;
	/** Size of the image in bits*/
	int img_size;
	/** Rsiz*/
	OPJ_RSIZ_CAPABILITIES rsiz;
	/** Enabling Tile part generation*/
	char tp_on;
	/** Flag determining tile part generation*/
	char tp_flag;
	/** Position of tile part flag in progression order*/
	int tp_pos;
	/** allocation by rate/distortion */
	int disto_alloc;
	/** allocation by fixed layer */
	int fixed_alloc;
	/** add fixed_quality */
	int fixed_quality;
	/** if != 0, then original dimension divided by 2^(reduce); if == 0 or not used, image is decoded to the full resolution */
	int reduce;
	/** if != 0, then only the first "layer" layers are decoded; if == 0 or not used, all the quality layers are decoded */
	int layer;
	/** if == NO_LIMITATION, decode entire codestream; if == LIMIT_TO_MAIN_HEADER then only decode the main header */
	OPJ_LIMIT_DECODING limit_decoding;
	/** XTOsiz */
	int tx0;
	/** YTOsiz */
	int ty0;
	/** XTsiz */
	int tdx;
	/** YTsiz */
	int tdy;
	/** comment for coding */
	char *comment;
	/** number of tiles in width */
	int tw;
	/** number of tiles in heigth */
	int th;
	/** ID number of the tiles present in the codestream */
	int *tileno;
	/** size of the vector tileno */
	int tileno_size;
	/** packet header store there for futur use in t2_decode_packet */
	unsigned char *ppm_data;
	/** pointer remaining on the first byte of the first header if ppm is used */
	unsigned char *ppm_data_first;
	/** if ppm == 1 --> there was a PPM marker for the present tile */
	int ppm;
	/** use in case of multiple marker PPM (number of info already store) */
	int ppm_store;
	/** use in case of multiple marker PPM (case on non-finished previous info) */
	int ppm_previous;
	/** ppmbug1 */
	int ppm_len;
	/** tile coding parameters */
	opj_tcp_t *tcps;
	/** fixed layer */
	int *matrice;
/* UniPG>> */
#ifdef USE_JPWL
	/** enables writing of EPC in MH, thus activating JPWL */
	opj_bool epc_on;
	/** enables writing of EPB, in case of activated JPWL */
	opj_bool epb_on;
	/** enables writing of ESD, in case of activated JPWL */
	opj_bool esd_on;
	/** enables writing of informative techniques of ESD, in case of activated JPWL */
	opj_bool info_on;
	/** enables writing of RED, in case of activated JPWL */
	opj_bool red_on;
	/** error protection method for MH (0,1,16,32,37-128) */
	int hprot_MH;
	/** tile number of header protection specification (>=0) */
	int hprot_TPH_tileno[JPWL_MAX_NO_TILESPECS];
	/** error protection methods for TPHs (0,1,16,32,37-128) */
	int hprot_TPH[JPWL_MAX_NO_TILESPECS];
	/** tile number of packet protection specification (>=0) */
	int pprot_tileno[JPWL_MAX_NO_PACKSPECS];
	/** packet number of packet protection specification (>=0) */
	int pprot_packno[JPWL_MAX_NO_PACKSPECS];
	/** error protection methods for packets (0,1,16,32,37-128) */
	int pprot[JPWL_MAX_NO_PACKSPECS];
	/** enables writing of ESD, (0/2/4 bytes) */
	int sens_size;
	/** sensitivity addressing size (0=auto/2/4 bytes) */
	int sens_addr;
	/** sensitivity range (0-3) */
	int sens_range;
	/** sensitivity method for MH (-1,0-7) */
	int sens_MH;
	/** tile number of sensitivity specification (>=0) */
	int sens_TPH_tileno[JPWL_MAX_NO_TILESPECS];
	/** sensitivity methods for TPHs (-1,0-7) */
	int sens_TPH[JPWL_MAX_NO_TILESPECS];
	/** enables JPWL correction at the decoder */
	opj_bool correct;
	/** expected number of components at the decoder */
	int exp_comps;
	/** maximum number of tiles at the decoder */
	int max_tiles;
#endif /* USE_JPWL */
/* <<UniPG */
} opj_cp_t;

typedef struct opj_encoding_param
{
	/** Digital cinema profile*/
	OPJ_CINEMA_MODE m_cinema;
	/** Maximum rate for each component. If == 0, component size limitation is not considered */
	OPJ_UINT32 m_max_comp_size;
	/** Position of tile part flag in progression order*/
	OPJ_INT32 m_tp_pos;
	/** fixed layer */
	OPJ_INT32 *m_matrice;
	/** Flag determining tile part generation*/
	OPJ_BYTE m_tp_flag;
	/** allocation by rate/distortion */
	OPJ_UINT32 m_disto_alloc : 1;
	/** allocation by fixed layer */
	OPJ_UINT32 m_fixed_alloc : 1;
	/** add fixed_quality */
	OPJ_UINT32 m_fixed_quality : 1;
	/** Enabling Tile part generation*/
	OPJ_UINT32 m_tp_on : 1;
}
opj_encoding_param_t;

typedef struct opj_decoding_param
{
	/** if != 0, then original dimension divided by 2^(reduce); if == 0 or not used, image is decoded to the full resolution */
	OPJ_UINT32 m_reduce;
	/** if != 0, then only the first "layer" layers are decoded; if == 0 or not used, all the quality layers are decoded */
	OPJ_UINT32 m_layer;
}
opj_decoding_param_t;


/**
Coding parameters
*/
typedef struct opj_cp_v2
{
	/** Size of the image in bits*/
	/*int img_size;*/
	/** Rsiz*/
	OPJ_RSIZ_CAPABILITIES rsiz;
	/** XTOsiz */
	OPJ_UINT32 tx0; // MSD see norm
	/** YTOsiz */
	OPJ_UINT32 ty0; // MSD see norm
	/** XTsiz */
	OPJ_UINT32 tdx;
	/** YTsiz */
	OPJ_UINT32 tdy;
	/** comment */
	OPJ_CHAR *comment;
	/** number of tiles in width */
	OPJ_UINT32 tw;
	/** number of tiles in heigth */
	OPJ_UINT32 th;




	/** packet header store there for futur use in t2_decode_packet */
	OPJ_BYTE *ppm_data;
	/** size of the ppm_data*/
	OPJ_UINT32 ppm_len;
	/** size of the ppm_data*/
	OPJ_UINT32 ppm_data_read;

	OPJ_BYTE *ppm_data_current;

	/** packet header storage original buffer */
	OPJ_BYTE *ppm_buffer;
	/** pointer remaining on the first byte of the first header if ppm is used */
	OPJ_BYTE *ppm_data_first;
	/** Number of bytes actually stored inside the ppm_data */
	OPJ_UINT32 ppm_data_size;
	/** use in case of multiple marker PPM (number of info already store) */
	OPJ_INT32 ppm_store;
	/** use in case of multiple marker PPM (case on non-finished previous info) */
	OPJ_INT32 ppm_previous;

	/** tile coding parameters */
	opj_tcp_v2_t *tcps;

	union
	{
		opj_decoding_param_t m_dec;
		opj_encoding_param_t m_enc;
	}
	m_specific_param;


/* UniPG>> */
#ifdef USE_JPWL
	/** enables writing of EPC in MH, thus activating JPWL */
	opj_bool epc_on;
	/** enables writing of EPB, in case of activated JPWL */
	opj_bool epb_on;
	/** enables writing of ESD, in case of activated JPWL */
	opj_bool esd_on;
	/** enables writing of informative techniques of ESD, in case of activated JPWL */
	opj_bool info_on;
	/** enables writing of RED, in case of activated JPWL */
	opj_bool red_on;
	/** error protection method for MH (0,1,16,32,37-128) */
	int hprot_MH;
	/** tile number of header protection specification (>=0) */
	int hprot_TPH_tileno[JPWL_MAX_NO_TILESPECS];
	/** error protection methods for TPHs (0,1,16,32,37-128) */
	int hprot_TPH[JPWL_MAX_NO_TILESPECS];
	/** tile number of packet protection specification (>=0) */
	int pprot_tileno[JPWL_MAX_NO_PACKSPECS];
	/** packet number of packet protection specification (>=0) */
	int pprot_packno[JPWL_MAX_NO_PACKSPECS];
	/** error protection methods for packets (0,1,16,32,37-128) */
	int pprot[JPWL_MAX_NO_PACKSPECS];
	/** enables writing of ESD, (0/2/4 bytes) */
	int sens_size;
	/** sensitivity addressing size (0=auto/2/4 bytes) */
	int sens_addr;
	/** sensitivity range (0-3) */
	int sens_range;
	/** sensitivity method for MH (-1,0-7) */
	int sens_MH;
	/** tile number of sensitivity specification (>=0) */
	int sens_TPH_tileno[JPWL_MAX_NO_TILESPECS];
	/** sensitivity methods for TPHs (-1,0-7) */
	int sens_TPH[JPWL_MAX_NO_TILESPECS];
	/** enables JPWL correction at the decoder */
	opj_bool correct;
	/** expected number of components at the decoder */
	int exp_comps;
	/** maximum number of tiles at the decoder */
	int max_tiles;
#endif /* USE_JPWL */

	/******** FLAGS *********/
	/** if ppm == 1 --> there was a PPM marker*/
	OPJ_UINT32 ppm : 1;
	/** tells if the parameter is a coding or decoding one */
	OPJ_UINT32 m_is_decoder : 1;
/* <<UniPG */
} opj_cp_v2_t;


typedef struct opj_j2k_dec
{
	/** locate in which part of the codestream the decoder is (main header, tile header, end) */
	OPJ_UINT32 m_state;
	/**
	 * store decoding parameters common to all tiles (information like COD, COC in main header)
	 */
	opj_tcp_v2_t *m_default_tcp;
	OPJ_BYTE  *m_header_data;
	OPJ_UINT32 m_header_data_size;
	/** to tell the tile part length */
	OPJ_UINT32 m_sot_length;
	/** Only tiles index in the correct range will be decoded.*/
	OPJ_UINT32 m_start_tile_x;
	OPJ_UINT32 m_start_tile_y;
	OPJ_UINT32 m_end_tile_x;
	OPJ_UINT32 m_end_tile_y;
	/**
	 * Indicate that the current tile-part is assume as the last tile part of the codestream.
	 * It is useful in the case of PSot is equal to zero. The sot length will be compute in the
	 * SOD reader function. FIXME NOT USED for the moment
	 */
	opj_bool   m_last_tile_part;
	/** to tell that a tile can be decoded. */
	OPJ_UINT32 m_can_decode			: 1;
	OPJ_UINT32 m_discard_tiles		: 1;
	OPJ_UINT32 m_skip_data			: 1;

} opj_j2k_dec_t;

typedef struct opj_j2k_enc
{
	/** Tile part number, regardless of poc, for each new poc, tp is reset to 1*/
	OPJ_UINT32 m_current_poc_tile_part_number; // tp_num

	/** Tile part number currently coding, taking into account POC. m_current_tile_part_number holds the total number of tile parts while encoding the last tile part.*/
	OPJ_UINT32 m_current_tile_part_number; //cur_tp_num

	/**
	locate the start position of the TLM marker
	after encoding the tilepart, a jump (in j2k_write_sod) is done to the TLM marker to store the value of its length.
	*/
	OPJ_SIZE_T m_tlm_start;
	/**
	 * Stores the sizes of the tlm.
	 */
	OPJ_BYTE * m_tlm_sot_offsets_buffer;
	/**
	 * The current offset of the tlm buffer.
	 */
	OPJ_BYTE * m_tlm_sot_offsets_current;

	/** Total num of tile parts in whole image = num tiles* num tileparts in each tile*/
	/** used in TLMmarker*/
	OPJ_UINT32 m_total_tile_parts;	 // totnum_tp

	/* encoded data for a tile */
	OPJ_BYTE * m_encoded_tile_data;

	/* size of the encoded_data */
	OPJ_UINT32 m_encoded_tile_size;

	/* encoded data for a tile */
	OPJ_BYTE * m_header_tile_data;

	/* size of the encoded_data */
	OPJ_UINT32 m_header_tile_data_size;


} opj_j2k_enc_t;

/**
JPEG-2000 codestream reader/writer
*/
typedef struct opj_j2k {
	/** codec context */
	opj_common_ptr cinfo;

	/** locate in which part of the codestream the decoder is (main header, tile header, end) */
	int state;
	/** number of the tile curently concern by coding/decoding */
	int curtileno;
	/** Tile part number*/
	int tp_num;
	/** Tilepart number currently coding*/
	int cur_tp_num;
	/** Total number of tileparts of the current tile*/
	int *cur_totnum_tp;
	/**
	locate the start position of the TLM marker  
	after encoding the tilepart, a jump (in j2k_write_sod) is done to the TLM marker to store the value of its length. 
	*/
	int tlm_start;
	/** Total num of tile parts in whole image = num tiles* num tileparts in each tile*/
	/** used in TLMmarker*/
	int totnum_tp;	
	/** 
	locate the position of the end of the tile in the codestream, 
	used to detect a truncated codestream (in j2k_read_sod)
	*/
	unsigned char *eot;
	/**
	locate the start position of the SOT marker of the current coded tile:  
	after encoding the tile, a jump (in j2k_write_sod) is done to the SOT marker to store the value of its length. 
	*/
	int sot_start;
	int sod_start;
	/**
	as the J2K-file is written in several parts during encoding, 
	it enables to make the right correction in position return by cio_tell
	*/
	int pos_correction;
	/** array used to store the data of each tile */
	unsigned char **tile_data;
	/** array used to store the length of each tile */
	int *tile_len;
	/** 
	decompression only : 
	store decoding parameters common to all tiles (information like COD, COC in main header)
	*/
	opj_tcp_t *default_tcp;
	/** pointer to the encoded / decoded image */
	opj_image_t *image;
	/** pointer to the coding parameters */
	opj_cp_t *cp;
	/** helper used to write the index file */
	opj_codestream_info_t *cstr_info;
	/** pointer to the byte i/o stream */
	opj_cio_t *cio;
} opj_j2k_t;

struct opj_tcd_v2;
/**
JPEG-2000 codestream reader/writer
*/
typedef struct opj_j2k_v2
{
	/* J2K codestream is decoded*/
	opj_bool m_is_decoder;

	/* FIXME DOC*/
	union
	{
		opj_j2k_dec_t m_decoder;
		opj_j2k_enc_t m_encoder;
	}
	m_specific_param;

	/** pointer to the encoded / decoded image */
	//opj_image_t *m_image;
	opj_image_t* m_image;

	/** Coding parameters */
	opj_cp_v2_t m_cp;

	/** the list of procedures to exec **/
	struct opj_procedure_list *	m_procedure_list;

	/** the list of validation procedures to follow to make sure the code is valid **/
	struct opj_procedure_list *	m_validation_list;

	/** helper used to write the index file */
	opj_codestream_index_t *cstr_index;

	/** number of the tile curently concern by coding/decoding */
	OPJ_UINT32 m_current_tile_number;

	/** the current tile coder/decoder **/
	struct opj_tcd_v2 *	m_tcd;
	//opj_tcd_v2_t *	m_tcd;



}
opj_j2k_v2_t;




/** @name Exported functions */
/*@{*/
/* ----------------------------------------------------------------------- */
/**
Creates a J2K decompression structure
@param cinfo Codec context info
@return Returns a handle to a J2K decompressor if successful, returns NULL otherwise
*/
opj_j2k_t* j2k_create_decompress(opj_common_ptr cinfo);
/**
Destroy a J2K decompressor handle
@param j2k J2K decompressor handle to destroy
*/
void j2k_destroy_decompress(opj_j2k_t *j2k);
/**
Setup the decoder decoding parameters using user parameters.
Decoding parameters are returned in j2k->cp. 
@param j2k J2K decompressor handle
@param parameters decompression parameters
*/
void j2k_setup_decoder(opj_j2k_t *j2k, opj_dparameters_t *parameters);

void j2k_setup_decoder_v2(opj_j2k_v2_t *j2k, opj_dparameters_t *parameters);

/**
Decode an image from a JPEG-2000 codestream
@param j2k J2K decompressor handle
@param cio Input buffer stream
@param cstr_info Codestream information structure if required, NULL otherwise
@return Returns a decoded image if successful, returns NULL otherwise
*/
opj_image_t* j2k_decode(opj_j2k_t *j2k, opj_cio_t *cio, opj_codestream_info_t *cstr_info);
/**
Decode an image form a JPT-stream (JPEG 2000, JPIP)
@param j2k J2K decompressor handle
@param cio Input buffer stream
@param cstr_info Codestream information structure if required, NULL otherwise
@return Returns a decoded image if successful, returns NULL otherwise
*/
opj_image_t* j2k_decode_jpt_stream(opj_j2k_t *j2k, opj_cio_t *cio, opj_codestream_info_t *cstr_info);
/**
Creates a J2K compression structure
@param cinfo Codec context info
@return Returns a handle to a J2K compressor if successful, returns NULL otherwise
*/
opj_j2k_t* j2k_create_compress(opj_common_ptr cinfo);

/**
Creates a J2K compression structure
@param cinfo Codec context info
@return Returns a handle to a J2K compressor if successful, returns NULL otherwise
*/
opj_j2k_v2_t* j2k_create_compress_v2();

/**
Destroy a J2K compressor handle
@param j2k J2K compressor handle to destroy
*/
void j2k_destroy_compress(opj_j2k_t *j2k);
/**
Setup the encoder parameters using the current image and using user parameters. 
Coding parameters are returned in j2k->cp. 
@param j2k J2K compressor handle
@param parameters compression parameters
@param image input filled image
*/
void j2k_setup_encoder(opj_j2k_t *j2k, opj_cparameters_t *parameters, opj_image_t *image);
/**
Converts an enum type progression order to string type
*/
char *j2k_convert_progression_order(OPJ_PROG_ORDER prg_order);
/**
Encode an image into a JPEG-2000 codestream
@param j2k J2K compressor handle
@param cio Output buffer stream
@param image Image to encode
@param cstr_info Codestream information structure if required, NULL otherwise
@return Returns true if successful, returns false otherwise
*/
opj_bool j2k_encode(opj_j2k_t *j2k, opj_cio_t *cio, opj_image_t *image, opj_codestream_info_t *cstr_info);

/* ----------------------------------------------------------------------- */
/*@}*/

/*@}*/

/**
 * Ends the decompression procedures and possibiliy add data to be read after the
 * codestream.
 */
opj_bool j2k_end_decompress(opj_j2k_v2_t *j2k, struct opj_stream_private *cio, struct opj_event_mgr * p_manager);

/**
 * Reads a jpeg2000 codestream header structure.
 *
 * @param cio the stream to read data from.
 * @param p_j2k the jpeg2000 codec.
 * @param p_manager the user event manager.
 *
 * @return true if the box is valid.
 */
opj_bool j2k_read_header(	struct opj_stream_private *p_stream,
							opj_j2k_v2_t* p_j2k,
							opj_image_t* p_image,
							struct opj_event_mgr* p_manager );


/**
 * Destroys a jpeg2000 codec.
 *
 * @param	p_j2k	the jpeg20000 structure to destroy.
 */
void j2k_destroy (opj_j2k_v2_t *p_j2k);

/**
 * Decode tile data.
 * @param	p_j2k		the jpeg2000 codec.
 * @param	p_stream			the stream to write data to.
 * @param	p_manager	the user event manager.
 */
opj_bool j2k_decode_tile (
					opj_j2k_v2_t * p_j2k,
					OPJ_UINT32 p_tile_index,
					OPJ_BYTE * p_data,
					OPJ_UINT32 p_data_size,
					struct opj_stream_private *p_stream,
					struct opj_event_mgr * p_manager
					);

/**
 * Reads a tile header.
 * @param	p_j2k		the jpeg2000 codec.
 * @param	p_stream			the stream to write data to.
 * @param	p_manager	the user event manager.
 */
opj_bool j2k_read_tile_header (
					 opj_j2k_v2_t * p_j2k,
					 OPJ_UINT32 * p_tile_index,
					 OPJ_UINT32 * p_data_size,
					 OPJ_INT32 * p_tile_x0,
					 OPJ_INT32 * p_tile_y0,
					 OPJ_INT32 * p_tile_x1,
					 OPJ_INT32 * p_tile_y1,
					 OPJ_UINT32 * p_nb_comps,
					 opj_bool * p_go_on,
					 struct opj_stream_private *p_stream,
					 struct opj_event_mgr * p_manager
					);


/**
 * Sets the given area to be decoded. This function should be called right after opj_read_header and before any tile header reading.
 *
 * @param	p_j2k			the jpeg2000 codec.
 * @param	p_start_x		the left position of the rectangle to decode (in image coordinates).
 * @param	p_end_x			the right position of the rectangle to decode (in image coordinates).
 * @param	p_start_y		the up position of the rectangle to decode (in image coordinates).
 * @param	p_end_y			the bottom position of the rectangle to decode (in image coordinates).
 * @param	p_manager		the user event manager
 *
 * @return	true			if the area could be set.
 */
opj_bool j2k_set_decode_area(	opj_j2k_v2_t *p_j2k,
								OPJ_INT32 p_start_x, OPJ_INT32 p_start_y,
								OPJ_INT32 p_end_x, OPJ_INT32 p_end_y,
								struct opj_event_mgr * p_manager );

/**
 * Creates a J2K decompression structure.
 *
 * @return a handle to a J2K decompressor if successful, NULL otherwise.
 */
opj_j2k_v2_t* j2k_create_decompress_v2();


/**
 * Dump some elements from the J2K decompression structure .
 *
 *@param p_j2k				the jpeg2000 codec.
 *@param flag				flag to describe what elments are dump.
 *@param out_stream			output stream where dump the elements.
 *
*/
void j2k_dump (opj_j2k_v2_t* p_j2k, OPJ_INT32 flag, FILE* out_stream);



/**
 * Dump an image header structure.
 *
 *@param img_header			the image header to dump.
 *@param dev_dump_flag		flag to describe if we are in the case of this function is use outside j2k_dump function
 *@param out_stream			output stream where dump the elements.
 */
void j2k_dump_image_header(opj_image_t* image, opj_bool dev_dump_flag, FILE* out_stream);

/**
 * Dump a component image header structure.
 *
 *@param comp_header		the component image header to dump.
 *@param dev_dump_flag		flag to describe if we are in the case of this function is use outside j2k_dump function
 *@param out_stream			output stream where dump the elements.
 */
void j2k_dump_image_comp_header(opj_image_comp_t* comp, opj_bool dev_dump_flag, FILE* out_stream);

/**
 * Get the codestream info from a JPEG2000 codec.
 *
 *@param	p_j2k				the component image header to dump.
 *
 *@return	the codestream information extract from the jpg2000 codec
 */
opj_codestream_info_v2_t* j2k_get_cstr_info(opj_j2k_v2_t* p_j2k);

/**
 * Get the codestream index from a JPEG2000 codec.
 *
 *@param	p_j2k				the component image header to dump.
 *
 *@return	the codestream index extract from the jpg2000 codec
 */
opj_codestream_index_t* j2k_get_cstr_index(opj_j2k_v2_t* p_j2k);

/**
 * Decode an image from a JPEG-2000 codestream
 * @param j2k J2K decompressor handle
 * @param cio Input buffer stream
 * @param cstr_info Codestream information structure if required, NULL otherwise
 * @return Returns a decoded image if successful, returns NULL otherwise
*/
opj_bool j2k_decode_v2(opj_j2k_v2_t *j2k, struct opj_stream_private *cio, opj_image_t* p_image, opj_event_mgr_t * p_manager);


#endif /* __J2K_H */
