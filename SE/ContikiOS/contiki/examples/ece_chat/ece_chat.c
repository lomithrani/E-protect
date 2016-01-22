#include <stdio.h>
#include "contiki.h"
#include "dev/serial-line.h"
#include "shell.h"
#include "serial-shell.h"
#include "shell-chat.h"

PROCESS(ece_chat, "Shell process");
AUTOSTART_PROCESSES(&ece_chat);

PROCESS_THREAD(ece_chat, ev, data)
{
	PROCESS_BEGIN();

	serial_line_init();
	serial_shell_init();

	chat_init();

	shell_reboot_init();
	shell_power_init();
	shell_ps_init();
	shell_ping_init();

	PROCESS_END();
}
