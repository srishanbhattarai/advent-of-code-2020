#include <cstdint>
#include <cstdio>
#include <vector>

using namespace std;

using i64 = int64_t;

// input
const i64 ts = 1003240;
const vector<i64> ids = {
    19, 41, 37,  787, 13,
    23, 29, 571, 17}; // I just removed the x's from the text

int main() {
  i64 closest = 1 << 20; // any sufficiently large num
  i64 closest_id = ids[0];

  for (auto id : ids) {
    i64 closest_for_id = ((ts + id) / id) * id;
    if (closest_for_id < closest) {
      closest = closest_for_id;
      closest_id = id;
    }
  }
  i64 wait_time = closest - ts;

  printf("id=%lld, wait_time=%lld, answer=%lld\n", closest_id, wait_time,
         closest_id * wait_time);

  return 0;
}
