#ifndef ARMOR_PARAM_H
#define ARMOR_PARAM_H
class ArmorParam
    {
        private:
        //pre-treatment
        int brightness_threshold = 210;
        int color_threshold = 40;
        float light_color_detect_extend_ratio = 1.1;

        // Filter lights
        float light_min_area = 10;
        float light_max_angle = 45.0;
        float light_min_size = 5.0;
        float light_contour_min_solidity = 0.5;
        float light_max_ratio = 1.0;

        // Filter pairs
        float light_max_angle_diff_ = 7.0; //20
        float light_max_height_diff_ratio_ = 0.2; //0.5
        float light_max_y_diff_ratio_ = 2.0; //100
        float light_min_x_diff_ratio_ = 0.5; //100

        // Filter armor
        float armor_big_armor_ratio = 3.2;
        float armor_small_armor_ratio = 2;
        //armor_max_height_ = 100.0;
        //armor_max_angle_ = 30.0;
        float armor_min_aspect_ratio_ = 1.0;
        float armor_max_aspect_ratio_ = 5.0;

        //other params
        int sight_offset_normalized_base = 200;
        int area_normalized_base = 1000;
        //RGB enemy_color = BLUE;
    };
#endif // ARMOR_PARAM_H
