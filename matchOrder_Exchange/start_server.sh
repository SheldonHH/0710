#!/bin/bash

# 获取当前时间戳
timestamp=$(date +"%Y%m%d_%H%M%S")

# 定义日志文件名
output_log="server_output_$timestamp.log"
error_log="server_error_$timestamp.log"

# 运行客户端并将输出和错误日志保存到带有时间戳的文件中，同时在屏幕上打印日志
./server > >(tee $output_log) 2> >(tee $error_log >&2)
