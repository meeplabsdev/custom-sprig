if [ $# -eq 0 ]; then
    echo "Usage: $0 <input.png>"
    exit 1
fi

input="$1"
ffmpeg -i "$input" -vf scale=160:128 -pix_fmt bgr565be -f rawvideo "${input}.bin"
