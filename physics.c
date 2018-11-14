#include "physics.h"
#include <stdio.h>
#include <float.h>
#include <GLFW/glfw3.h>

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

	struct HIT_DETECT_CALC block_calc_result;
	/*if (!hit_detect_calc(ray, block, &temp_calc_result)) {
		return 0;
	}*/
	CHECK_RETURN(hit_detect_calc(ray, block, &block_calc_result));

	if (block->visual_effect.is_visible) {
		*calc_out = block_calc_result;
		*hit_block = block;
		return 1;
	}

	struct HIT_DETECT_CALC current_child_result;
	struct HIT_DETECT_CALC best_child_result;
	struct WORLD_BLOCK* child_hit_block = NULL;
	struct WORLD_BLOCK* best_child_hit_block = NULL;
	int is_child_hit = 0;
	//printf("BLK %d %d %d %d\n", block->position.x, block->position.y, block->position.z, block->level);
	for (int i = 0; i < 8; i++) {
		int result = hit_detect(ray, block->children[i], &current_child_result, &child_hit_block);
		if (result) {
			if (!is_child_hit) {
				is_child_hit = 1;
				best_child_result = current_child_result;
				best_child_hit_block = child_hit_block;
				//printf("-> BLK %d %d %d %d [%d] %d %lf\n", block->position.x, block->position.y, block->position.z, block->level, i, is_child_hit, current_child_result.result);
			}
			else if (current_child_result.result < best_child_result.result) {
				best_child_result = current_child_result;
				best_child_hit_block = child_hit_block;
				//printf("=> BLK %d %d %d %d [%d] %d %lf\n", block->position.x, block->position.y, block->position.z, block->level, i, is_child_hit, current_child_result.result);
			}
		}
	}

	if (is_child_hit) {
		*calc_out = best_child_result;
		*hit_block = best_child_hit_block;
		return 1;
	}

	return 0;

	//output_info->hit_block = block;
	//output_info->
}

int hit_count = 0;
void ray_cast(struct  RAY* ray, struct WORLD_BLOCK* root, struct RAY_HIT_INFO* output_info) {
	struct HIT_DETECT_CALC calc_result;
	struct WORLD_BLOCK* hit_block = NULL;
	//printf("BEGIN >---------------------------\n");
	hit_detect(ray, root, &calc_result, &hit_block);

	output_info->hit_block = hit_block;

	if (hit_block) {
		//printf("HIT %d: X %d Y %d Z %d L %d\n\n", hit_count++, hit_block->position.x, hit_block->position.y, hit_block->position.z, hit_block->level);
		if (calc_result.result == calc_result.tx_in) {
			if (calc_result.result == calc_result.tx1) { output_info->hit_face = 0; }
			else { output_info->hit_face = 1; }
		}
		else if (calc_result.result == calc_result.ty_in) {
			if (calc_result.result == calc_result.ty1) { output_info->hit_face = 2; }
			else { output_info->hit_face = 3; }
		}
		else if (calc_result.result == calc_result.tz_in) {
			if (calc_result.result == calc_result.tz1) { output_info->hit_face = 4; }
			else { output_info->hit_face = 5; }
		}
		else {
			//ERROR
			printf("ERROR");
		}

		//output_info->hit_position = ray->start_pos*(1 - calc_result.result) + ray->end_pos*calc_result.result;
		output_info->hit_position.x = ray->start_pos.x*(1 - calc_result.result) + ray->end_pos.x*calc_result.result;
		output_info->hit_position.y = ray->start_pos.y*(1 - calc_result.result) + ray->end_pos.y*calc_result.result;
		output_info->hit_position.z = ray->start_pos.z*(1 - calc_result.result) + ray->end_pos.z*calc_result.result;

	}

}


void draw_block_hit_normal(struct RAY_HIT_INFO* hit_info) {

	//printf("\rF: %d  L:%d    <%d,%d,%d>", hit_info->hit_face, hit_info->hit_block->level, hit_info->hit_block->position.x, hit_info->hit_block->position.y, hit_info->hit_block->position.z);

	if (hit_info->hit_block) {
		struct VECTOR3DF l1 = hit_info->hit_position;
		struct VECTOR3DF l2 = hit_info->hit_position;
		switch (hit_info->hit_face)
		{
		case 0:
			l2.x -= 1;
			break;
		case 1:
			l2.x += 1;
			break;
		case 2:
			l2.y -= 1;
			break;
		case 3:
			l2.y += 1;
			break;
		case 4:
			l2.z -= 1;
			break;
		case 5:
			l2.z += 1;
			break;
		default:
			break;
		}
		glBegin(GL_LINES);
		glColor3f(1, 1, 1);
		glVertex3f(l1.x, l1.y, l1.z);
		glVertex3f(l2.x, l2.y, l2.z);
		glEnd();
	}
}

