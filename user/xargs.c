#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h" // Chứa hằng số MAXARG

int main(int argc, char *argv[]) {
// 1. Kiểm tra số lượng đối số
if (argc < 2) {
fprintf(2, "Usage: xargs [args...]\n");
exit(1);
}

char *cmd = argv[1]; // Lệnh cần chạy (ví dụ: echo) [cite: 25]
char *new_argv[MAXARG]; // Mảng chứa toàn bộ đối số cho lệnh mới
char buf[512]; // Bộ đệm để đọc dữ liệu từ stdin
int i;

// 2. Copy các đối số ban đầu từ dòng lệnh vào new_argv
// Bỏ qua 'xargs', bắt đầu từ argv[1]
for (i = 1; i < argc; i++) {
new_argv[i - 1] = argv[i];
}

// 3. Đọc dữ liệu từ Standard Input (file descriptor 0)
while (1) {
int index = 0;
char ch;
int n;

// Đọc từng ký tự cho đến khi gặp newline (\n)
while ((n = read(0, &ch, 1)) > 0) {
if (ch == '\n') {
break;
}
buf[index++] = ch;
}

// Nếu không đọc được thêm dữ liệu (hết file), dừng vòng lặp
if (n <= 0 && index == 0) {
break;
}

// Kết thúc chuỗi đọc được từ dòng
buf[index] = '\0';

// 4. Tạo tiến trình con để thực thi lệnh
if (fork() == 0) {
// Trong tiến trình con:
// Thêm đối số vừa đọc từ dòng vào cuối danh sách đối số
new_argv[argc - 1] = buf;
new_argv[argc] = 0; // Kết thúc mảng bằng NULL

// Thực thi lệnh với các đối số mới
exec(cmd, new_argv);

// Nếu exec thất bại
fprintf(2, "xargs: exec %s failed\n", cmd);
exit(1);
} else {
// Trong tiến trình cha:
// Đợi tiến trình con hoàn thành rồi mới đọc dòng tiếp theo
wait(0);
}
}

exit(0);
}