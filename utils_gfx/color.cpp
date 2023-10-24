#include "color.h"

#include <math.h>

/* Classic color distance declaration */
inline u_int32_t distance_rgb( u_int16_t* RGB1, u_int16_t* RGB2 );
inline u_int16_t get_closest_value(u_int8_t* RGB_ptr, int16_t max_colors);

/* RGB2LAB declarations */
double main_vdi_palette_lab[256][3];
double deltaE(double* labA, double* labB);
u_int16_t get_closest_value_rgb2lab(u_int8_t* RGB_ptr, int16_t max_colors);

/* Classic color distance routines */

inline u_int32_t distance_rgb( u_int16_t* RGB1, u_int16_t* RGB2 ) {
    u_int32_t rez = 0;

    u_int32_t r = (RGB1[0] - RGB2[0]);
    u_int32_t g = (RGB1[1] - RGB2[1]);
    u_int32_t b = (RGB1[2] - RGB2[2]);

    u_int32_t drp2 = (r*r);
    u_int32_t dgp2 = (g*g);
    u_int32_t dbp2 = (b*b);    

    u_int32_t t = (RGB1[0] + RGB2[0]) >> 1;

    rez = sqrt((drp2 << 1) + (dgp2 << 2) + mul_3_fast(dbp2) + t * (drp2 - dbp2) >> 8);

    return rez;
}

inline u_int16_t get_closest_value(u_int8_t* RGB_ptr, int16_t max_colors) {
    u_int16_t i = 0;
    u_int32_t j = 0, best_idx = 0;

    u_int16_t better_distance = 0xFFFF;

    u_int16_t pal_value[3];

    u_int16_t RGB[3];

    RGB[0] = RGB_ptr[0];
    RGB[1] = RGB_ptr[1];
    RGB[2] = RGB_ptr[2];    

	while(i < max_colors ) {

        switch (computer_type)
        {
        case 0:
            pal_value[0] = ((palette_ori[i] >> 8) & 0x0F ) << 5 ;
            pal_value[1] = ((palette_ori[i] >> 4) & 0x0F ) << 5 ;
            pal_value[2] = ((palette_ori[i]) & 0x0F ) << 5 ;            
            break;
        default:
            pal_value[0] =  ( ((palette_ori[i] >> 7) & 0x0E ) | ((palette_ori[i] >> 11) & 0x01 ) ) << 4;
            pal_value[1] = ( ((palette_ori[i] >> 3) & 0x0E) | ((palette_ori[i] >> 7) & 0x01 ) ) << 4;
            pal_value[2] = ( ((palette_ori[i]) & 0x07 ) << 1 | ((palette_ori[i] >> 3) & 0x01 ) ) << 4;
            break;
        }

        j = distance_rgb(RGB,pal_value);

        if( j < better_distance ){
            better_distance = j;
            best_idx = i;
        }
        i++;
	}

    return best_idx;
}

void classic_RGB_to_8bits_Indexed(u_int8_t* src_ptr, u_int8_t* dst_ptr, int16_t width, int16_t height, int16_t max_colors){
    u_int32_t totalPixels = mul_3_fast(MFDB_STRIDE(width) * height);
    u_int32_t i = 0;
    while(i < totalPixels){
        *dst_ptr++ = (u_int8_t)get_closest_value(&src_ptr[i], max_colors);
        i = i + 3;
    }
}
