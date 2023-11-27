#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main()
{
    uid_t current_uid = getuid();
    printf("현재 UID: %d\n", current_uid);

    uid_t new_uid = 1001;
    // 0일 경우 성공한다.
    if (setuid(new_uid) == 0)
    {
        printf("UID를 %d로 변경했습니다.\n", new_uid);

        uid_t updated_uid = getuid();
        printf("변경된 UID: %d\n", updated_uid);
    }
    else
    {
        perror("setuid");
        return 1;
    }

    return 0;
}
