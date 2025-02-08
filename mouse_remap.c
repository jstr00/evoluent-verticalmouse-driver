#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <linux/uinput.h>
#include <errno.h>
#include <libevdev-1.0/libevdev/libevdev.h>

#define CONFIG_FILE "/etc/mouse_remap/config.txt"
#define DEVICE_NAME "Kingsis Peripherals Evoluent VerticalMouse D"
#define EVENT_PATH "/proc/bus/input/devices"

struct ButtonMap
{
    int input_button;
    int output_key;
};

struct ButtonMap button_map[] = {
    {272, BTN_LEFT},
    {274, BTN_RIGHT},
    {276, 0},
    {273, BTN_RIGHT},
    {275, KEY_BACK},
    {277, KEY_FORWARD}};

char *get_device_path()
{
    static char path[64];
    FILE *file = fopen(EVENT_PATH, "r");
    char line[256], event[32];
    int found = 0;

    if (!file)
        return NULL;

    while (fgets(line, sizeof(line), file))
    {
        if (strstr(line, DEVICE_NAME))
            found = 1;
        if (found && sscanf(line, "H: Handlers=%*s event%31s", event) == 1)
        {
            snprintf(path, sizeof(path), "/dev/input/event%s", event);
            fclose(file);
            return path;
        }
    }
    fclose(file);
    return NULL;
}

void load_config()
{
    FILE *file = fopen(CONFIG_FILE, "r");
    if (!file)
        return;

    char line[256];
    while (fgets(line, sizeof(line), file))
    {
        char button[32], key[32];
        if (sscanf(line, "%31[^=]=%31s", button, key) == 2)
        {
            int btn_code = libevdev_event_code_from_name(EV_KEY, button);
            int key_code = libevdev_event_code_from_name(EV_KEY, key);
            if (btn_code > 0 && key_code > 0)
            {
                for (size_t i = 0; i < sizeof(button_map) / sizeof(button_map[0]); i++)
                {
                    if (button_map[i].input_button == btn_code)
                    {
                        button_map[i].output_key = key_code;
                        break;
                    }
                }
            }
        }
    }
    fclose(file);
}

int main(void)
{
    int src_fd, uinp_fd;
    struct input_event ev;
    ssize_t n;
    char *src_dev = get_device_path();

    if (!src_dev)
        return EXIT_FAILURE;

    load_config();

    src_fd = open(src_dev, O_RDONLY);
    if (src_fd < 0)
        return EXIT_FAILURE;

    uinp_fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
    if (uinp_fd < 0)
    {
        close(src_fd);
        return EXIT_FAILURE;
    }

    ioctl(uinp_fd, UI_SET_EVBIT, EV_KEY);
    for (size_t i = 0; i < sizeof(button_map) / sizeof(button_map[0]); i++)
    {
        ioctl(uinp_fd, UI_SET_KEYBIT, button_map[i].output_key);
    }

    struct uinput_user_dev uidev;
    memset(&uidev, 0, sizeof(uidev));
    snprintf(uidev.name, UINPUT_MAX_NAME_SIZE, "Evoluent_D_Driver");
    uidev.id.bustype = BUS_USB;
    uidev.id.vendor = 0x0001;
    uidev.id.product = 0x0001;
    uidev.id.version = 1;

    ssize_t bytes_written;
    bytes_written = write(uinp_fd, &uidev, sizeof(uidev));
    if (bytes_written != sizeof(uidev))
    {
        perror("Error writing uidev");
    }
    ioctl(uinp_fd, UI_DEV_CREATE);

    while ((n = read(src_fd, &ev, sizeof(ev))) == sizeof(ev))
    {
        if (ev.type == EV_KEY)
        {
            for (size_t i = 0; i < sizeof(button_map) / sizeof(button_map[0]); i++)
            {
                if (ev.code == button_map[i].input_button)
                {
                    ev.code = button_map[i].output_key;
                    break;
                }
            }
        }
        bytes_written = write(uinp_fd, &ev, sizeof(ev));
        if (bytes_written != sizeof(ev))
        {
            perror("Error writing event");
        }
    }

    ioctl(uinp_fd, UI_DEV_DESTROY);
    close(uinp_fd);
    close(src_fd);
    return EXIT_SUCCESS;
}
