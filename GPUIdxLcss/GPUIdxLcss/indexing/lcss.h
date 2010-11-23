#ifndef _LCSS_H_
#define _LCSS_H_

#include <vector>

// 标准LCSS
int LCSS( std::vector<int> &vec1, std::vector<int>  &vec2);

// 带约束的LCSS，
int LCSSDelta(std::vector<int>  &vec1, std::vector<int> &vec2, int delta);

#endif 