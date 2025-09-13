#!/usr/bin/env bash
# ==========================================
#  STM32F411 串口一键擦除 + 烧录 + 运行
#  依赖: stm32flash (sudo apt install stm32flash)
# ==========================================

# 默认参数
PORT="${1:-/dev/ttyUSB0}"   # 第一个参数给串口，默认 /dev/ttyUSB0
HEX="${2:-build/Debug/*.hex}" # 第二个参数给 hex 文件，支持通配
HEX=$(ls $HEX 2>/dev/null | head -n1)   # 自动取第一个 hex

[[ -z "$HEX" ]] && { echo "❌ 找不到 hex 文件，请先编译或手动指定路径"; exit 1; }
[[ ! -r "$HEX" ]] && { echo "❌ 文件不存在: $HEX"; exit 1; }
[[ ! -w "$PORT" ]] && { echo "❌ 串口不可写: $PORT (试试 sudo ?)"; exit 1; }

echo "=================================="
echo "串口 : $PORT"
echo "HEX  : $HEX"
echo "=================================="

# ---- 1. 全局擦除 ----
echo "🔥 正在全局擦除 ..."
stm32flash -o "$PORT" || { echo "❌ 擦除失败"; exit 2; }

# ---- 2. 烧录 + 校验 ----
echo "📤 正在烧录并校验 ..."
stm32flash -w "$HEX" -v -g 0x0 "$PORT" || { echo "❌ 烧录失败"; exit 3; }

echo "✅ 一切搞定，芯片已自动重启运行！"