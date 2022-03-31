#include "main.h"

Point operator+(const Point& point1, const Point& point2) {
  return {point1.x + point2.x, point1.y + point2.y};
}

Image::Image(const std::string& input_path) {
  // Загружаю изображение, чтобы получить информацию о ширине, высоте и цветовом канале
  image_ = stbi_load(input_path.c_str(), &rows_, &columns_, &channel_, 1);
  if (image_ == nullptr) {
    std::cout << "Cannot open the picture";
    exit(0);
  }
  data_ = std::vector(rows_, std::vector<int>(columns_));

  for (int i = 0; i < rows_; ++i) {
    for (int j = 0; j < columns_; ++j) {
      data_[i][j] = (int)*(image_ + (j + rows_ * i));
    }
  }
}

Image::~Image() {
  stbi_image_free(image_);
}

int Image::GetColumns() const { return columns_; }
int Image::GetRows() const { return rows_; }
unsigned char* Image::GetImage() const { return image_; }

void Image::CheckGradient(Point start_point, std::vector<std::vector<int>>& used,
                         std::vector<std::vector<int>>& new_data)  {
  int colors_shift = 0;
  std::queue<Point> queue;
  queue.push(start_point);
  std::vector<Point> gradient;
  std::vector<Point> directions = {{-1, 0}, {0, -1}, {1, 0}, {0, 1}};

  while (!queue.empty()) {
    Point top = queue.front();
    queue.pop();

    for (auto dir: directions) {
      Point neighbour = top + dir;

      // вышли за пределы картинки или пиксель уже был посещён
      if (neighbour.y < 0 || neighbour.y >= columns_ ||
          neighbour.x < 0 || neighbour.x >= rows_ ||
          used[neighbour.x][neighbour.y]) {
        continue;
      }

      // если тон сменился не более, чем на 3, то добавляю
      if (top.val - 5 <= neighbour.val && neighbour.val <= top.val + 5) {
        queue.push(neighbour);
        used[neighbour.x][neighbour.y] = 1;
        if (top.val != neighbour.val) {
          ++colors_shift;
        }
      }
    }

    used[top.x][top.y] = 1;
    gradient.push_back(top);
  }

  // если сдвигов в цвете не было, то это был однотонный цвет без градиента,
  // иначе был градиент - добавляю в ответ
  if (colors_shift != 0) {
    for (auto point: gradient) {
      new_data[point.x][point.y] = color_;
    }
    color_ += 3;
  }
}

std::vector<std::vector<int>> Image::Gradient() {
  std::vector<std::vector<int>> used(rows_, std::vector<int>(columns_, 0));
  std::vector<std::vector<int>> new_data(columns_, std::vector<int>(rows_, 0));
  for (int i = 0; i < columns_; ++i) {
    for (int j = 0; j < rows_; ++j) {
      if (!used[i][j]) {
        CheckGradient({i, j}, used, new_data);
      }
    }
  }
  return new_data;
}

void DrawGradient(std::vector<std::vector<int>>& new_data, Image& image,
                  const std::string& output_path) {
  int rows = image.GetRows();
  int columns = image.GetColumns();
  unsigned char* img = image.GetImage();
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < columns; ++j) {
      *(img + (j + rows * i)) = new_data[i][j];
    }
  }
  int comp;
  stbi_write_png(output_path.c_str(), rows, columns, comp, img, 1);
}

int main() {
  std::cin >> input_path;
  Image image(input_path);

  std::vector<std::vector<int>> new_data = image.Gradient();
  std::string output_path = "answer.png";
  DrawGradient(new_data, image, output_path);
}
