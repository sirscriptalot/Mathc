#include <stdbool.h>
#include "mathc/mathc.h"


/** Transform a 3d point with a transformation matrix M */
vec3 transform_point(mat4 M, vec3 point) {
    // Create a homogenous 3d point
    vec4 hpoint;
    hpoint.xyz = point;
    // 1 for points, 0 for vectors
    hpoint.w = 1;

    // return only xyz=vec3 of the resulting vec4 point
    return mat4_mul_vec(M, hpoint).xyz;
}

/** Transform a 3d vector with a transformation matrix M */
vec3 transform_vector(mat4 M, vec3 vector) {
    // vectors need (M^-1)^t for transformations
    // (if the matrix uses scaling and sheering)
    // its safe to override (copies)
    M = mat4_transpose(mat4_inv(M));
    vec4 hvector;
    hvector.xyz = vector;
    hvector.w = 0;
    return mat4_mul_vec(M, hvector).xyz;
}

typedef struct Sphere_s {
    float x, y, z, radius;
} Sphere_s;

/** Test collision between spheres */
bool sphere_collision(Sphere_s a, Sphere_s b) {
    // _v functions take const float * and treats them as vec3
    vec3 dist = vec3_sub_vev_v(&b.x, &a.x);

    // norm(dist) < a.r+b.r
    // powf(,2) is much faster than sqrt
    return vec3_dot(dist, dist) < powf(a.r + b.r, 2);
}

/** calculates the angle between vector a and b */
float vec_angle(vec3 a, vec3 b) {
    // Safe to override (copies)
    a = vec3_normalize(a);
    b = vec3_normalize(b);
    return acosf(vec3_dot(a, b));
}

/** creates a pose from a ray. dir -> Z axis */
mat4 ray_to_pose(vec3 pos, vec3 dir) {
    vec3 z = vec3_normalize(dir);

    // align x axis of pose to unit x
    vec3 y = vec3_cross(z, (vec3) VEC3_INIT_UNIT_X);
    if(vec3_norm(y) < 0.01f) {
        // z is parallel to x, using y for x instead
        y = vec3_cross(z, (vec3) VEC3_INIT_UNIT_Y);
    }
    y = vec3_normalize(y);

    vec3 x = vec3_cross(y, z);
    // no need to normalize, cause y and z are orthogonal and normalized

    mat4 pose = MAT4_INIT_EYE;
    pose.col[0].xyz = x;
    pose.col[1].xyz = y;
    pose.col[2].xyz = z;
    pose.col[3].xyz = pos;
    return pose;
}

/** creates a camera VP matrix */
mat4 create_camera_VP(vec3 eye, vec3 dir, vec3 up, bool orhto) {
    vec3 center = vec3_add_vec(eye, dir);
    mat4 V = mat4_camera_lookat(eye, center, up);
    mat4 P;
    if(orhto)
        P = mat4_camera_ortho(-1, 1, -1, 1, -2, 10);
    else
        P = mat4_camera_frustum(-1, 1, -1, 1, -2, 10);
    return mat4_mul_mat(P, V);
}

int main() {
    mat4 pose = ray_to_pose((vec3) {{100, 100, 50}}, (vec3) {{0, 0, 1}});
    vec3 point = {{10, 20, 30}};
    point = transform_point(mat4_inv(pose), point);
    vec3_print(point);

    mat4 VP = create_camera_VP((vec3) {{10, 20, 30}},
                               vec3_neg((vec3) VEC3_INIT_UNIT_Z),
                               (vec3) VEC3_INIT_UNIT_Y,
                               false);
    vec3 normal = {{0, 1, 0}};
    normal = transform_vector(VP, normal);
    vec3_print(normal);
}