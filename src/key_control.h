#ifndef KEY_CONTROL_H
#define KEY_CONTROL_H 1

#define KEY_NONE      -1
#define KEY_UNKNOWN   0
#define KEY_UP        0x1b5b41
#define KEY_DOWN      0x1b5b42
#define KEY_RIGHT     0x1b5b43
#define KEY_LEFT      0x1b5b44
#define KEY_ENTER     0x0a
#define KEY_ESC       0x1b
#define KEY_BACKSPACE 0x7f

#if defined(__cplusplus)
   extern "C" {
#endif

int set_icanon(void);

int unset_icanon(void);

int init_key_control(void);

int end_key_control(void);

int get_key(void);

#if defined(__cplusplus)
   }
#endif

#endif

