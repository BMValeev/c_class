/* Define to prevent recursive inclusion ------------------------------------ */
#ifndef BATTERY_H
#define BATTERY_H

/* ******************************************************************************** */
/*        INTERNAL PARAMETERS                                                       */
/*   TO BE ADJUSTED ACCORDING TO BATTERY AND APPLCICATION CHARACTERISTICS           */
/* -------------------------------------------------------------------------------- */

/*Battery parameters define  ------------------------------------------------------ */
#define CAPACITY		1500	/* battery nominal capacity in mAh					*/
#define RINT			200	/* Internal battery impedance in mOhms,0 if unknown	*/

#define OCV_OFFSET_TAB	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} /* OCVTAB	*/
	
/*Application parameters define  -------------------------------------------------- */
#define VMODE 			MIXED_MODE	/* running mode constant, VM_MODE or MIXED_MODE	*/
#define ALM_EN			0		/* Alarm enable constant, set at 1 to enable	*/
#define ALM_SOC			10		/* SOC alarm in % 								*/
#define ALM_VBAT 		3600		/* Voltage alarm in mV							*/
#define RSENSE			10		/* sense resistor in mOhms 						*/

#define APP_EOC_CURRENT       75   		/* end charge current in mA                 */
#define APP_CUTOFF_VOLTAGE	  3000   	/* application cut-off voltage in mV      	*/

/* ******************************************************************************** */

#endif // BATTERY_H
