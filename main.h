#include <iostream>
#include <string>
#include <vector>
#include <queue>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image_resize.h"

typedef struct Point {
  int x = 0;
  int y = 0;
  int val = 0;
} Point;

Point operator+(const Point& point1, const Point& point2);

class Image {
 private:
  int color_ = 50;
  int rows_ = 0;
  int columns_ = 0;
  int channel_ = 0;
  unsigned char* image_;
  std::vector<std::vector<int>> data_;

 public:
  explicit Image(const std::string& input_path);
  ~Image();
  void CheckGradient(Point start_point, std::vector<std::vector<int>>& used,
                     std::vector<std::vector<int>>& new_data);
  std::vector<std::vector<int>> Gradient();

  int GetRows() const;
  int GetColumns() const;
  unsigned char* GetImage() const;
};

void DrawGradient(std::vector<std::vector<int>>& new_data, Image& image,
                  const std::string& output_path);
