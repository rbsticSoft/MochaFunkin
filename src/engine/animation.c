#include "animation.h"
#include "sprite.h"

#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <string.h>
#include <stdlib.h>

fnf_animation* find_animation(fnf_animation_controller* controller, const char* name){
    for(int i=0; i<controller->collection->animationsSize; i++){
        int found = strcmp(controller->collection->animations[i].name, name);
        if(found == 0) 
            return &controller->collection->animations[i];
    }
    return NULL; // not found
}

fnf_animation_prefix* find_prefix(fnf_animation_controller* controller, const char* name){
    for(int i=0; i<controller->collection->prefixesSize; i++){
        int found = strcmp(controller->collection->prefixes[i].name, name);
        if(found == 0) 
            return &controller->collection->prefixes[i];
    }
    return NULL; // not found
}

int32 xmlGetIntProp(const xmlNode *node, const char* prop){
    xmlChar* sprop = xmlGetProp(node, prop);
    if(!sprop) return 0;
    return strtol(sprop, NULL, 10);
}

bool animation_load_atlas(fnf_animation_controller* controller, const char* path){
    if(!controller)
        return false;

    xmlDoc *doc = NULL;
    xmlNode *root_element = NULL, *atlas_element = NULL;

    doc = xmlReadFile(path, NULL, 0);
    root_element = xmlDocGetRootElement(doc);
    atlas_element = xmlFirstElementChild(root_element);
    do
    {
        unsigned char* framenum;
        xmlChar* animName = xmlGetProp(atlas_element, "name");

        int sframeIndex = strlen(animName)-4;
        int prevValue = animName[sframeIndex];

        animName[sframeIndex] = 0;
        fnf_animation* animation = find_animation(controller, animName);
        if(!animation){
            animation = &controller->collection->animations[controller->collection->animationsSize++];
            strcpy(animation->name, animName);
        }
        animName[sframeIndex] = prevValue;

        framenum = &animName[sframeIndex];

        int32  animX = xmlGetIntProp(atlas_element, "x");
        int32  animY = xmlGetIntProp(atlas_element, "y");
        int32  animFX = xmlGetIntProp(atlas_element, "frameX");
        int32  animFY = xmlGetIntProp(atlas_element, "frameY");

        int32  animW = xmlGetIntProp(atlas_element, "width");
        int32  animH = xmlGetIntProp(atlas_element, "height");
        int32  animFW = xmlGetIntProp(atlas_element, "frameWidth");
        int32  animFH = xmlGetIntProp(atlas_element, "frameHeight");
        
        animation->frames[animation->framesSize].x = animX;
        animation->frames[animation->framesSize].y = animY;
        animation->frames[animation->framesSize].w = animW;
        animation->frames[animation->framesSize].h = animH;

        animation->frames[animation->framesSize].fx = animFX;
        animation->frames[animation->framesSize].fy = animFY;
        animation->frames[animation->framesSize].fw = animFW;
        animation->frames[animation->framesSize].fh = animFH;

        animation->framesSize++;
        
    }
    while( (atlas_element = xmlNextElementSibling(atlas_element)) );

    printf("Element: %s\n", root_element->name);

    xmlFreeDoc(doc);
    xmlCleanupParser();

    return true;
}

bool animation_add_prefix(fnf_animation_controller *controller, const char *sprefix, const char *anim, bool looped, int32 fps)
{
    fnf_animation* animation;

    if(!(animation = find_animation(controller, anim))) return false;

    fnf_animation_prefix* prefix = &controller->collection->prefixes[controller->collection->prefixesSize++];
    strcpy(prefix->name, sprefix);
    prefix->fps = fps;
    prefix->looped = looped;
    prefix->animation = animation;
    for(int i=0; i<prefix->animation->framesSize; i++)
        prefix->playlist[i] = i;
    prefix->playlistSize = prefix->animation->framesSize;
    prefix->playlist[prefix->animation->framesSize] = -1;

    return true;
}

bool animation_add_prefix_indices(fnf_animation_controller* controller, const char* sprefix, const char* anim, bool looped, int32 fps, int32* indices, uint8 indices_size)
{
    fnf_animation* animation;

    if(!(animation = find_animation(controller, anim))) return false;

    fnf_animation_prefix* prefix = &controller->collection->prefixes[controller->collection->prefixesSize++];
    strcpy(prefix->name, sprefix);
    prefix->fps = fps;
    prefix->looped = looped;
    prefix->animation = animation;
    memcpy(prefix->playlist, indices, indices_size * sizeof(int32));
    prefix->playlistSize = indices_size;
    prefix->playlist[indices_size] = -1;

    return true;
}

#include <SDL2/SDL.h>

bool animation_play(fnf_animation_controller* animation, const char* prefix) {
    fnf_animation_prefix* anim = find_prefix(animation, prefix);
    if(!anim) return;
    //if(animation->currentAnimation == anim && !animation->finished)
    //    return false;

    animation->frameNum = 0;
    animation->currentAnimation = anim;
    animation->finished = false;
    animation->lastFrame = SDL_GetTicks();
    return animation->currentAnimation != NULL;
}

bool animation_set_offset(fnf_animation_controller* animation, const char* prefix, float x, float y){
    fnf_animation_prefix* anim = find_prefix(animation, prefix);
    if(!anim)
        return false;

    anim->offset.x = -x;
    anim->offset.y = -y;

    return true;
}