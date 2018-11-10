# Ray to AABB hit detection

```
#include <stdio.h>
#include <float.h>

int get_in_out_t(double* tx, double* x0, double* bx1, double* bx2, double* t_in_x, double* t_out_x){
    if(*tx==0){
        if(*x0<*bx1 || *x0>*bx2)
            return 0;
        else{
            *t_in_x=DBL_MIN;
            *t_out_x=DBL_MAX;
        }
    }
    else{
        *t_in_x=(*bx1-*x0)/(*tx);
        *t_out_x=(*bx2-*x0)/(*tx);
        if(*t_in_x>*t_out_x){
            double temp=*t_in_x;
            *t_in_x=*t_out_x;
            *t_out_x=temp;
        }
    }
    return 1;
}


int ray_aabb(double x0, double y0, double z0,
double tx, double ty, double tz,
double bx1, double bx2, double by1, double by2, double bz1, double bz2,
double* t_in, double* t_out){
    double t_in_x, t_in_y, t_in_z, t_out_x, t_out_y, t_out_z;

    if(!get_in_out_t(&tx, &x0, &bx1, &bx2, &t_in_x, &t_out_x)){
        return 0;
    }

    if(!get_in_out_t(&ty, &y0, &by1, &by2, &t_in_y, &t_out_y)){
        return 0;
    }

    if(!get_in_out_t(&tz, &z0, &bz1, &bz2, &t_in_z, &t_out_z)){
        return 0;
    }

    double max_in=t_in_x;
    if(t_in_y>max_in){
        max_in=t_in_y;
    }
    if(t_in_z>max_in){
        max_in=t_in_z;
    }
    double min_out=t_out_x;
    if(t_out_y<min_out){
        min_out=t_out_y;
    }
    if(t_out_z<min_out){
        min_out=t_out_z;
    }

    if(max_in<min_out){
        return 1;
    }
    return 0;

}
```
