#pragma once
#include "../features.hpp"
#include "../../../source-sdk/sdk.hpp"

class Chams
{
public:
    void OnDrawModelExecute(i_mat_render_context* ctx, const draw_model_state_t& state, const model_render_info_t& pInfo, matrix_t* pCustomBoneToWorld);

private:
    void OverrideMaterial(bool ignoreZ, const color& rgba);
    i_material* materialFlat = nullptr;
    i_material* materialArmRace = nullptr;
};// extern Chams chams;
