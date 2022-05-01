/*******************************************
v * Solutions for the CS:APP Performance Lab
 ********************************************/

#include <stdio.h>
#include <stdlib.h>
#include "defs.h"

/* 
 * Please fill in the following student struct 
 */
student_t student = {
  "Ryan Brooks",     /* Full name */
  "u1115093@utah.edu",  /* Email address */
};

/***************
 * COMPLEX KERNEL
 ***************/

/******************************************************
 * Your different versions of the complex kernel go here
 ******************************************************/

/* 
 * naive_complex - The naive baseline version of complex 
 */
char naive_complex_descr[] = "naive_complex: Naive baseline implementation";
void naive_complex(int dim, pixel *src, pixel *dest)
{
  int i, j;

  for(i = 0; i < dim; i++)
    for(j = 0; j < dim; j++)
    {

      dest[RIDX(dim - j - 1, dim - i - 1, dim)].red = ((int)src[RIDX(i, j, dim)].red +
						      (int)src[RIDX(i, j, dim)].green +
						      (int)src[RIDX(i, j, dim)].blue) / 3;
      
      dest[RIDX(dim - j - 1, dim - i - 1, dim)].green = ((int)src[RIDX(i, j, dim)].red +
							(int)src[RIDX(i, j, dim)].green +
							(int)src[RIDX(i, j, dim)].blue) / 3;
      
      dest[RIDX(dim - j - 1, dim - i - 1, dim)].blue = ((int)src[RIDX(i, j, dim)].red +
						       (int)src[RIDX(i, j, dim)].green +
						       (int)src[RIDX(i, j, dim)].blue) / 3;

    }
}


/*
 * complex - Your current working version of complex
 * IMPORTANT: This is the version you will be graded on
 */
char complex_descr[] = "complex: Current working version";
void complex(int dim, pixel *src, pixel *dest)
{
  int i, j;
  for(i = 0; i < dim; i++)
    {
      int dimi = dim-i;
      for(j = 0; j < dim; j+=4)
	{

	  pixel *s = src+RIDX(j,i,dim);
	pixel *d = dest+RIDX(dimi,-1-j,dim);

        d->red =
        d->blue =
          d->green = (int)(s->red+s->green+s->blue)/3;

        (d-1)->red =
	  (d-1)->blue =
          (d-1)->green = (int)((s+dim)->red+(s+dim)->green+(s+dim)->blue)/3;

        (d-2)->red =
          (d-2)->blue =
          (d-2)->green = (int)((s+dim+dim)->red+(s+dim+dim)->green+(s+dim+dim)->blue)/3;

	(d-3)->red =
		(d-3)->blue =
		(d-3)->green = (int)((s+dim+dim+dim)->red+(s+dim+dim+dim)->green+(s+dim+dim+dim)->blue)/3;

	}
    }
}


/*********************************************************************
 * register_complex_functions - Register all of your different versions
 *     of the complex kernel with the driver by calling the
 *     add_complex_function() for each test function. When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_complex_functions() {
  add_complex_function(&complex, complex_descr);
  add_complex_function(&naive_complex, naive_complex_descr);
}


/***************
 * MOTION KERNEL
 **************/

/***************************************************************
 * Various helper functions for the motion kernel
 * You may modify these or add new ones any way you like.
 **************************************************************/


/* 
 * weighted_combo - Returns new pixel value at (i,j) 
 */
static pixel weighted_combo(int dim, int i, int j, pixel *src) 
{
  int ii, jj;
  pixel current_pixel;

  int red, green, blue;
  red = green = blue = 0;

  int num_neighbors = 0;
  for(ii=0; ii < 3; ii++)
    for(jj=0; jj < 3; jj++) 
      if ((i + ii < dim) && (j + jj < dim)) 
      {
	num_neighbors++;
	red += (int) src[RIDX(i+ii,j+jj,dim)].red;
	green += (int) src[RIDX(i+ii,j+jj,dim)].green;
	blue += (int) src[RIDX(i+ii,j+jj,dim)].blue;
      }
  
  current_pixel.red = (unsigned short) (red / num_neighbors);
  current_pixel.green = (unsigned short) (green / num_neighbors);
  current_pixel.blue = (unsigned short) (blue / num_neighbors);
  
  return current_pixel;
}

static pixel weighted_combo2(int dim, int i, int j, pixel *src)
{
  int ii, jj;
  pixel current_pixel;

  int red, green, blue;
  red = green = blue = 0;

  int num_neighbors = 0;
  for(jj=0; jj < 3; jj++)
    for(ii=0; ii < 3; ii++)
      if ((i + ii < dim) && (j + jj < dim))
	{
	  pixel * s = &src[RIDX(i+ii,j+jj,dim)];
	  num_neighbors++;
	  red += (int) s->red;
	  green += (int) s->green;
	  blue += (int) s->blue;
	}

  current_pixel.red = (unsigned short) (red / num_neighbors);
  current_pixel.green = (unsigned short) (green / num_neighbors);
  current_pixel.blue = (unsigned short) (blue / num_neighbors);

  return current_pixel;
}

/******************************************************
 * Your different versions of the motion kernel go here
 ******************************************************/


/*
 * naive_motion - The naive baseline version of motion 
 */
char naive_motion_descr[] = "naive_motion: Naive baseline implementation";
void naive_motion(int dim, pixel *src, pixel *dst) 
{
  int i, j;
    
  for (i = 0; i < dim; i++)
    for (j = 0; j < dim; j++)
      dst[RIDX(i, j, dim)] = weighted_combo(dim, i, j, src);
}


/*
 * motion - Your current working version of motion. 
 * IMPORTANT: This is the version you will be graded on
 */
char motion_descr[] = "motion: Current working version";
void motion(int dim, pixel *src, pixel *dst) 
{
  int i, j, ii, jj;
  i=j=0;
  for (i = 0; i < dim-2; i++)
    for (j = 0; j < dim-2; j++){
      int idx = RIDX(i,j,dim);
      pixel * p = &dst[idx];
      pixel * s = &src[idx];
      p->red=(unsigned short)((s->red+
		    (s+1)->red+
		    (s+2)->red+
		    (s+dim)->red+
		    (s+dim+1)->red+
		    (s+dim+2)->red+
		    (s+dim+dim)->red+
		    (s+dim+dim+1)->red+
		    (s+dim+dim+2)->red) /9);
      p->green=(unsigned short)((s->green+
		      (s+1)->green+
		      (s+2)->green+
		      (s+dim)->green+
		      (s+dim+1)->green+
		      (s+dim+2)->green+
		      (s+dim+dim)->green+
		      (s+dim+dim+1)->green+
		      (s+dim+dim+2)->green) /9);
      p->blue=(unsigned short)((s->blue+
		      (s+1)->blue+
		      (s+2)->blue+
		      (s+dim)->blue+
		      (s+dim+1)->blue+
		      (s+dim+2)->blue+
		      (s+dim+dim)->blue+
		      (s+dim+dim+1)->blue+
		      (s+dim+dim+2)->blue)/9);
  }
  for (ii=0; ii < i; ii++){
    int idx = RIDX(ii,j,dim);
    pixel * p = &dst[idx];
    pixel * s = &src[idx];
    p->red=(unsigned short)((s->red+
                             (s+1)->red+
                             (s+dim)->red+
                             (s+dim+1)->red+
                             (s+dim+dim)->red+
                             (s+dim+dim+1)->red)/6);
    p->green=(unsigned short)((s->green+
                               (s+1)->green+
                               (s+dim)->green+                              
                               (s+dim+1)->green+                            
                               (s+dim+dim)->green+
                               (s+dim+dim+1)->green)/6);
    p->blue=(unsigned short)((s->blue+
                              (s+1)->blue+
                              (s+dim)->blue+
                              (s+dim+1)->blue+
                              (s+dim+dim)->blue+
                              (s+dim+dim+1)->blue)/6);
    idx = RIDX(ii,j+1,dim);
    p = &dst[idx];
    s = &src[idx];
    p->red=(unsigned short)((s->red+
                             (s+dim)->red+
                             (s+dim+dim)->red)/3);
    p->green=(unsigned short)((s->green+
                               (s+dim)->green+
                               (s+dim+dim)->green)/3);
    p->blue=(unsigned short)((s->blue+
                              (s+dim)->blue+
                              (s+dim+dim)->blue)/3);
  }
  for (jj=0; jj < j; jj++){
    int idx = RIDX(i,jj,dim);
    pixel * p = &dst[idx];
    pixel * s = &src[idx];
    p->red=(unsigned short)((s->red+
                             (s+1)->red+
                             (s+2)->red+
                             (s+dim)->red+
                             (s+dim+1)->red+
                             (s+dim+2)->red)/6);
    p->green=(unsigned short)((s->green+
                               (s+1)->green+
                               (s+2)->green+
                               (s+dim)->green+
                               (s+dim+1)->green+
                               (s+dim+2)->green)/6);
    p->blue=(unsigned short)((s->blue+
                              (s+1)->blue+
                              (s+2)->blue+
                              (s+dim)->blue+
                              (s+dim+1)->blue+
                              (s+dim+2)->blue)/6);
    idx = RIDX(i+1,jj,dim);
    p = &dst[idx];
    s = &src[idx];
    p->red=(unsigned short)((s->red+
                             (s+1)->red+
                             (s+2)->red)/3);
    p->green=(unsigned short)((s->green+
                               (s+1)->green+
                               (s+2)->green)/3);
    p->blue=(unsigned short)((s->blue+
                              (s+1)->blue+
                              (s+2)->blue)/3);
  }


  for (ii=i; ii < dim; ii++)
    for (jj=j; jj < dim; jj++){
      dst[RIDX(ii, jj, dim)] = weighted_combo2(dim, ii, jj, src);
    }
}

/********************************************************************* 
 * register_motion_functions - Register all of your different versions
 *     of the motion kernel with the driver by calling the
 *     add_motion_function() for each test function.  When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_motion_functions() {
  add_motion_function(&motion, motion_descr);
  add_motion_function(&naive_motion, naive_motion_descr);
}
