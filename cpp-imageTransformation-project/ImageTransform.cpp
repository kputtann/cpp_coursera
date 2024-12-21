#include <iostream>
#include <cmath>
#include <cstdlib>

#include "uiuc/PNG.h"
#include "uiuc/HSLAPixel.h"
#include "ImageTransform.h"

/* ******************
(Begin multi-line comment...)

Write your name and email address in the comment space here:

Name:
Email:

(...end multi-line comment.)
******************** */

using uiuc::PNG;
using uiuc::HSLAPixel;

/**
 * Returns an image that has been transformed to grayscale.
 *
 * The saturation of every pixel is set to 0, removing any color.
 *
 * @return The grayscale image.
 */
PNG grayscale(PNG image) {
  /// This function is already written for you so you can see how to
  /// interact with our PNG class.
  for (unsigned x = 0; x < image.width(); x++) {
    for (unsigned y = 0; y < image.height(); y++) {
      HSLAPixel & pixel = image.getPixel(x, y);

      // `pixel` is a reference to the memory stored inside of the PNG `image`,
      // which means you're changing the image directly. No need to `set`
      // the pixel since you're directly changing the memory of the image.
      pixel.s = 0;
    }
  }

  return image;
}



/**
 * Returns an image with a spotlight centered at (`centerX`, `centerY`).
 *
 * A spotlight adjusts the luminance of a pixel based on the distance the pixel
 * is away from the center by decreasing the luminance by 0.5% per 1 pixel euclidean
 * distance away from the center.
 *
 * For example, a pixel 3 pixels above and 4 pixels to the right of the center
 * is a total of `sqrt((3 * 3) + (4 * 4)) = sqrt(25) = 5` pixels away and
 * its luminance is decreased by 2.5% (0.975x its original value).  At a
 * distance over 160 pixels away, the luminance will always decreased by 80%.
 * 
 * The modified PNG is then returned.
 *
 * @param image A PNG object which holds the image data to be modified.
 * @param centerX The center x coordinate of the crosshair which is to be drawn.
 * @param centerY The center y coordinate of the crosshair which is to be drawn.
 *
 * @return The image with a spotlight.
 */
PNG createSpotlight(PNG image, int centerX, int centerY) {
  //cout << centerX << " " << centerY << endl;
  //double testdist = sqrt(pow((430-centerX), 2) + pow((150-centerY),2));
  //cout << testdist << endl;
  //cout << "scaling = " <<  (1-(0.005 * testdist)) << endl;

  for (unsigned x = 0; x < image.width(); x++) {
    for (unsigned y = 0; y < image.height(); y++) {
      HSLAPixel & pixel = image.getPixel(x, y);
      
      int xdist = x - centerX, ydist = y - centerY;  // Need to store as int (signed) to avoid issues with subtracting int and unsigned
      double dist = sqrt(pow(xdist, 2) + pow(ydist,2));
      
      /*
      if (x < 452 && x > 450 && y < 149 && y > 147){
        cout <<"x, y =" << x << ", " << y << "; dist = " << dist << endl;
        int xdist = x - centerX, ydist = y - centerY;
        double testdist = sqrt(pow(xdist, 2) + pow(ydist,2));
        cout << testdist << endl;
      }
      */
      
      if (dist >= 160){
        pixel.l = 0.2 * pixel.l;
      } else {
        pixel.l = (1-(0.005 * dist)) * pixel.l;
      }
    }
  }
  return image;
  
}
 

/**
 * Returns a image transformed to Illini colors.
 *
 * The hue of every pixel is set to the a hue value of either orange or
 * blue, based on if the pixel's hue value is closer to orange than blue.
 *
 * @param image A PNG object which holds the image data to be modified.
 *
 * @return The illinify'd image.
**/
/** Notes: 
 * Hue of Orange = 11
 * Hue of blue = 216
 * Check hue of every pixel (in nexted for loop)
 * Reset the hue to orange or blue 
 * 0 -> h -> 360 
 * 0 -> 11 -> 216 -> 360
 * (216 - 11) / 2 = 205 / 2 = 102.5 
 * => Orange if h <= 102.5, Blue if h > 102.5
 * (360 + 11 - 216) / 2 = 155 / 2 = 77.5 
 * => Orange if h > 293.5, Blue if h =< 292.5
**/
PNG illinify(PNG image) {
  for (unsigned x = 0; x < image.width(); x++) {
    for (unsigned y = 0; y < image.height(); y++) {
      HSLAPixel & pixel = image.getPixel(x, y);
      if (pixel.h > 293.5 || pixel.h <= 77.5){
        pixel.h = 11; 
      }else{
        pixel.h = 216; 
      }
    }
  }
  return image;
}
 

/**
* Returns an immge that has been watermarked by another image.
*
* The luminance of every pixel of the second image is checked, if that
* pixel's luminance is 1 (100%), then the pixel at the same location on
* the first image has its luminance increased by 0.2.
*
* @param firstImage  The first of the two PNGs, which is the base image.
* @param secondImage The second of the two PNGs, which acts as the stencil.
*
* @return The watermarked image.
*/
PNG watermark(PNG firstImage, PNG secondImage) {
  for (unsigned x = 0; x < firstImage.width(); x++) {
    for (unsigned y = 0; y < firstImage.height(); y++) {
      HSLAPixel & firstpixel = firstImage.getPixel(x, y);
      HSLAPixel & secondpixel = secondImage.getPixel(x, y);
      if (secondpixel.l == 1.0){
        firstpixel.l = min(1.0, firstpixel.l + 0.2);
      }
    }
  }

  return firstImage;
}
