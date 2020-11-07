
#include "CCL.h"
#include "Set.h"

// int* get_neighbors(int image[], int width, int height) {
//   // TODO: can modify for 4 or 8 connected components
//
//   int neighbors[4] = {};
//
//   return neighbors;
// }

//                E , S, W , N
// const int DX[] = {+1, 0, -1, 0};
// const int DY[] = {0, +1, 0, -1};

//                W , N
const int DX[] = {-1, 0};
const int DY[] = { 0, -1};
const int NUM_DIRECTIONS = 2;

const bool DEBUG = true;
const int BACKGROUND = 0;

// assume background = 0
int* label_components(int image[], int width, int height) {


  int image_size = width * height;

  Set uf = Set();
  // std::vector<Node*> linked;

  // make a label for each pixel
  Node* labels[image_size];
  for (int i = 0; i < image_size; i++) {
    labels[i] = uf.MakeSet(i + 1);
  }

  int next_label = 0;

  // First Pass
  for (int row = 0; row < height; row++) {
    for (int col = 0; col < width; col++ ) {
      // if (DEBUG) {std::cout << image[row * width + col];}

      if (image[row * width + col] != BACKGROUND) {

        // Get neighbors

        std::vector<int> neighbors;

        for (int direction = 0; direction < NUM_DIRECTIONS; direction++) {
          int neighbor_index = (row + DY[direction]) * width + col + DX[direction];

          bool w_err = col == 0 && DX[direction] == -1;
          bool n_err = row == 0 && DY[direction] == -1;
          // bool e_err = col == (width - 1) && DX[direction] == 1;
          // bool s_err = row == (height - 1) && DY[direction] == 1;

          if (w_err || n_err /* ||  e_err  || s_err */) {
            continue;
          }

          if (image[neighbor_index] != BACKGROUND) {
            neighbors.push_back(neighbor_index);
          }

          // if (DEBUG) { std::cout << row << ',' << col << '|' << neighbor_index << ':' <<direction <<  '\n';}
        }

        if (neighbors.empty()) {
          // linked.push_back(uf.MakeSet(next_label));
          // labels[row * width + col] = next_label;
          // next_label++;
        } else {
          // Get neighbors labels and minimum value :
          std::vector<Node*> neighbors_labels;

          Node* minimum_label = labels[neighbors[0]];
          int minimum_label_representative = (*uf.Find(minimum_label)).value;

          // get minimum neighbor label
          for (auto neighbor_index : neighbors) {
            neighbors_labels.push_back(labels[neighbor_index]);

            Node* neighbor_label = uf.Find(labels[neighbor_index]);
            int neighbor_label_representative = (*neighbor_label).value;

            if (neighbor_label_representative < minimum_label_representative) {
              // minimum_label_value = neighbor_label_value;
              minimum_label = neighbor_label;
              minimum_label_representative = neighbor_label_representative;
            }

          }

          uf.Union(labels[row * width + col], minimum_label);

          for (auto label : neighbors_labels) {
            uf.Union(label, minimum_label);
          }
        }
        // std::cout << '\n';

      }
    }
    // std::cout << '\n';
  }

  int labeled[image_size] = {};

  std::map<int, int> sequential;
  int sequential_counter = 1;

  // Second Pass
  for (int row = 0; row < height; row++) {
    for (int col = 0; col < width; col++ ) {
      if (image[row * width + col] != BACKGROUND) {

        int global_label = (*uf.Find(labels[row * width + col])).value;
        if (sequential.find(global_label) == sequential.end()) {
          sequential[global_label] = sequential_counter;
          sequential_counter++;
        }

        labeled[row * width + col] = sequential[global_label];
        // std::cout << labeled[row * width + col] ;
      }
      // std::cout << ' ';
    }
    // std::cout << '\n';
  }

  return labeled;

  // for (int row = 0; row < height; row++) {
  //     for (int col = 0; col < width; col++ ) {
  //       if (image[row * width + col] != BACKGROUND) {
  //         std::cout << labeled[row * width + col];
  //       }
  //       std::cout << ' ';
  //     }
  //     std::cout << '\n';
  //   }

}

int main() {
  // Set uf = Set();
  //
  // for (int i = 0; i < 8; i++) {
  //   uf.MakeSet(i);
  // }
  //
  // std::cout << uf.SetCount() << '\n';
  // std::cout << uf.FindByValue(4)->value << '\n';
  //
  // uf.UnionByValues(3, 4);
  //
  // std::cout << uf.SetCount() << '\n';
  // std::cout << uf.FindByValue(4)->value << '\n';

  int array[] = {
    1,0,0,0,
    1,1,0,1,
    0,1,0,0,
    0,0,1,1
    };
  int width = 4;
  int height = 4;

  label_components(array, width, height);

  return 0;
}
