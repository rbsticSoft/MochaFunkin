#include "camera.h"
#include "types.h"

fnf_camera* make_camera(){
    fnf_camera* camera = FNF_NEW(fnf_camera);
    FNF_ZERO(camera, sizeof(fnf_camera));

    camera->zoom = 1.0f;
    
    return camera;
}