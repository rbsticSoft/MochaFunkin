#ifndef CAMERA_H
#define CAMERA_H

typedef struct {
    float x,y,zoom;
    int w,h;
} fnf_camera;

fnf_camera* make_camera();

#endif