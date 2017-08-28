#pragma once
#include<string>
#include<vector>

#define TEST_DIR "testDB\\"

// Preprocessing configs
#define GAUSSIAN_BLUR_SIZE 25
#define ADAPTIVE_THRESHOLD_BLOCK_SIZE 201
#define ADAPTIVE_THRESHOLD_C 20
#define DILATE_KERNEL_SIZE 31

// Verify character configs
#define MIN_WIDHT 75
#define MIN_HEIGHT 100
#define MIN_AREA 6500
#define MAX_RATIO 7

// Size for preprocessed char
#define SIZE 20

// SVM filename
#define SVM_FILENAME "model4.yml"

