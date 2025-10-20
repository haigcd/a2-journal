#!/bin/bash

# Exit immediately if a command exits with a non-zero status.
set -e

# TODO: don't harcode to my home directory
AC="java -jar /home/haig/Downloads/AppleCommander-12.0/app/cli-ac/build/libs/AppleCommander-ac-12.0.jar"

PRODOS_NAME=JOURNAL
DISK=journal.dsk

cl65 --standard c99 -t apple2 -o journal.bin journal.c menu.c editor.c tea.c file_utils.c util.c

${AC} -d ${DISK} ${PRODOS_NAME}
${AC} -as ${DISK} ${PRODOS_NAME} B < journal.bin

mame apple2p -flop1 ${DISK} -nomouse