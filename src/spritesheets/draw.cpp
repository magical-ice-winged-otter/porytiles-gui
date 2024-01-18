#include "spritesheets/spritesheets.hpp"
#include "spritesheets/draw.hpp"

Mat DrawUtil::loadImage(const std::filesystem::path& imageFile) {
    std::string input = imageFile.string();
    Mat img = imread(input, IMREAD_COLOR);
    if(img.empty()) {
        printf("Attempt to load image %s failed!", imageFile.string().c_str());
        return DrawUtil::DEAD_IMAGE;
    }
    return img;
}

void DrawUtil::scanImage(Mat& mat, std::function<std::optional<uchar*>(const DrawUtil::GeneratorPixel&)> modify) {
    // accept only char type matrices
    CV_Assert(mat.depth() == CV_8U);
    int channels = mat.channels();
    int nRows = mat.rows;
    int nCols = mat.cols;
    if (mat.isContinuous()) {
        nCols *= nRows;
        nRows = 1;
    }
    uchar* p;
    for(int i = 0; i < nRows; ++i) {
        p = mat.ptr<uchar>(i);
        for (int j = 0; j < nCols * channels; j = j + channels) {
            uchar* col_ptr = &p[j];

            uchar copy[channels];
            for (int c = 0; c < channels; ++c) {
                copy[c] = col_ptr[c];
            }

            DrawUtil::GeneratorPixel pixel = { col_ptr, channels};
            std::optional<uchar*> modified = modify(pixel);
            if (!modified.has_value()) continue;
            for (int c = 0; c < channels; ++c) {
                col_ptr[c] = modified.value()[c];
            }
        }
    }
}

//box[0] the offset (usually 0, 0)
//box[1] the actual box definition
void DrawUtil::scanSpriteBox(Mat& mat, int box[2][2], std::function<void(const Rect&)> boxConsumer) {
    int upperLeftX = std::min(box[0][0], box[1][0]);
    int upperLeftY = std::min(box[0][1], box[1][1]);
    int bottomRightX = std::max(box[0][0], box[1][0]);
    int bottomRightY = std::max(box[0][1], box[1][1]);

    for (int i = upperLeftY; i < mat.rows; i = i + bottomRightY) {
        for (int j = upperLeftX; j < mat.cols; j = j + bottomRightX) {
            printf("x: %d, y: %d\n", j, i);
            Rect spriteBox(j, i, bottomRightX - 1, bottomRightY - 1);
            boxConsumer(spriteBox);
        }
    }
}