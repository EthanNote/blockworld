#include "physics.h"
#include <stdio.h>
#include <float.h>

int get_in_out_t(double* tx, double* x0, double* bx1, double* bx2, double* tx1, double* tx2) {
	if (*tx == 0) {
		if (*x0<*bx1 || *x0>*bx2)
			return 0;
		else {
			*tx1 = DBL_MIN;
			*tx2 = DBL_MAX;
		}
	}
	else {
		*tx1 = (*bx1 - *x0) / (*tx);
		*tx2 = (*bx2 - *x0) / (*tx);

	}
	return 1;
}

#define UP_ASIGN(x1, x2, y1, y2) if(x1<x2){y1=x1;y2=x2;}else{y1=x2;y2=x1;}
#define CHECK_RETURN(x) if(!x){return 0;}

double max(double x1, double x2, double x3) {
	if (x1 > x2) {
		if (x1 > x3) {
			return x1;
		}
		else {
			return x3;
		}
	}
	else {
		if (x2 > x3) {
			return x2;
		}
		else {
			return x3;
		}
	}
}

double min(double x1, double x2, double x3) {
	if (x1 > x2) {
		if (x2 > x3) {
			return x3;
		}
		else {
			return x2;
		}
	}
	else {
		if (x1 > x3) {
			return x3;
		}
		else {
			return x1;
		}
	}
}



//int ray_aabb(double x0, double y0, double z0,
//	double tx, double ty, double tz,
//	double bx1, double bx2, double by1, double by2, double bz1, double bz2,
//	double* t_in, double* t_out,
//	int *cross_order_x, int *cross_order_y, int *cross_order_z
//	) {
//	double tx1, ty1, tz1, tx2, ty2, tz2;
//	double tx_in, tx_out, ty_in, ty_out, tz_in, tz_out;
//	
//
//	CHECK_RETURN(get_in_out_t(&tx, &x0, &bx1, &bx2, &tx1, &tx2));
//	CHECK_RETURN(get_in_out_t(&ty, &y0, &by1, &by2, &ty1, &ty2));
//	CHECK_RETURN(get_in_out_t(&tz, &z0, &bz1, &bz2, &tz1, &tz2));
//	 
//	UP_ASIGN(tx1, tx2, tx_in, tx_out);
//	UP_ASIGN(ty1, ty2, ty_in, ty_out);
//	UP_ASIGN(tz1, tz2, tz_in, tz_out);
//
//	if (max(tx_in, ty_in, tz_in) < min(tx_out, ty_out, tz_out)) {
//		return 1;
//	}
//	return 0;
//
//}

struct HIT_DETECT_CALC {
	double tx1, ty1, tz1, tx2, ty2, tz2;
	double tx_in, tx_out, ty_in, ty_out, tz_in, tz_out;
	double result;
};

int hit_detect_calc(struct RAY* ray, struct WORLD_BLOCK* block,
	struct HIT_DETECT_CALC* out) {
	double tx = ray->end_pos.x - ray->start_pos.x;
	double ty = ray->end_pos.y - ray->start_pos.y;
	double tz = ray->end_pos.z - ray->start_pos.z;

	double bx1 = block->position.x;
	double by1 = block->position.y;
	double bz1 = block->position.z;

	double size = 1 << block->level;

	double bx2 = block->position.x + size;
	double by2 = block->position.y + size;
	double bz2 = block->position.z + size;

	CHECK_RETURN(get_in_out_t(&tx, &ray->start_pos.x, &bx1, &bx2, &out->tx1, &out->tx2));
	CHECK_RETURN(get_in_out_t(&ty, &ray->start_pos.y, &by1, &by2, &out->ty1, &out->ty2));
	CHECK_RETURN(get_in_out_t(&tz, &ray->start_pos.z, &bz1, &bz2, &out->tz1, &out->tz2));

	UP_ASIGN(out->tx1, out->tx2, out->tx_in, out->tx_out);
	UP_ASIGN(out->ty1, out->ty2, out->ty_in, out->ty_out);
	UP_ASIGN(out->tz1, out->tz2, out->tz_in, out->tz_out);

	double t_in_max = max(out->tx_in, out->ty_in, out->tz_in);
	double t_out_min = min(out->tx_out, out->ty_out, out->tz_out);

	if (t_in_max < t_out_min) {
		out->result = t_in_max;
		return 1;
	}

	return 0;
}


int hit_detect(struct RAY* ray, struct WORLD_BLOCK* block, struct HIT_DETECT_CALC* calc_out, struct WORLD_BLOCK** hit_block) {
	if (!block) {
		return 0;
	}

	struct HIT_DETECT_CALC temp_calc_result;
	/*if (!hit_detect_calc(ray, block, &temp_calc_result)) {
		return 0;
	}*/
	CHECK_RETURN(hit_detect_calc(ray, block, &temp_calc_result));

	if (block->visual_effect.is_visible) {
		*calc_out = temp_calc_result;
		*hit_block = block;
		return 1;
	}

	struct HIT_DETECT_CALC current_child_result;
	int count = 0;
	for (int i = 0; i < 8; i++) {
		int result = hit_detect(ray, block->children[i], &current_child_result, hit_block);
		if (result) {
			if (!count || current_child_result.result < temp_calc_result.result) {
				temp_calc_result = current_child_result;
			}
			count++;
		}
	}

	if (count) {
		*calc_out = temp_calc_result;
		return 1;
	}

	return 0;

	//output_info->hit_block = block;
	//output_info->
}

int hit_count = 0;
void ray_cast(struct  RAY* ray, struct WORLD_BLOCK* root, struct RAY_HIT_INFO* output_info) {
	struct HIT_DETECT_CALC temp_calc_result;
	struct WORLD_BLOCK* hit_block = NULL;
	hit_detect(ray, root, &temp_calc_result, &hit_block);

	if (hit_block) {
		printf("\rHIT %d: X %d Y %d Z %d L %d     ",hit_count++, hit_block->position.x, hit_block->position.y, hit_block->position.z, hit_block->level);
	}

}