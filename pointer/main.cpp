#include <iostream>

int main() {
  int arr[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  int *ptr = arr;

  std::cout << *(ptr + 3) << '\n';
  return 0;
}
