#include <stdio.h>
#include <stdint.h>
#include <modbus.h>
#include <errno.h>
#include <stdlib.h>
#include <getopt.h>
#include "dps-bt.h"

/* Flag set by ‘--verbose’. */
static int verbose_flag;
uint16_t uwBaudRate;
uint16_t uwVolt;

int main(int argc, char *const *argv) {
    modbus_t *mb;
    uint16_t tab_reg[32];
    int i;
    int rc;
    int c;

    while (1)
    {
        static struct option long_options[] =
        {
            /* These options set a flag. */
            {"verbose", no_argument,       &verbose_flag, 1},
            /* These options don’t set a flag.
               We distinguish them by their indices. */
            {"baud",    required_argument, 0, 'b'},
            {"volt",    required_argument, 0, 'v'},
            {"append",  no_argument,       0, 'x'},
            {"delete",  required_argument, 0, 'd'},
            {"create",  required_argument, 0, 'c'},
            {"file",    required_argument, 0, 'f'},
            {0, 0, 0, 0}
        };
        /* getopt_long stores the option index here. */
        int option_index = 0;

        c = getopt_long (argc, argv, "ab:c:d:f:v:",
                         long_options, &option_index);

        /* Detect the end of the options. */
        if (c == -1)
            break;

        switch (c)
        {
        case 0:
            /* If this option set a flag, do nothing else now. */
            if (long_options[option_index].flag != 0)
                break;
            printf ("option %s", long_options[option_index].name);
            if (optarg)
                printf (" with arg %s", optarg);
            printf ("\n");
            break;

        case 'a':
            puts ("option -a\n");
            break;

        case 'b':
            uwBaudRate = (uint16_t)atoi(optarg);
            break;

        case 'v':
            uwVolt = (uint16_t)atoi(optarg);
            break;

        case 'c':
            printf ("option -c with value `%s'\n", optarg);
            break;

        case 'd':
            printf ("option -d with value `%s'\n", optarg);
            break;

        case 'f':
            printf ("option -f with value `%s'\n", optarg);
            break;

        case '?':
            /* getopt_long already printed an error message. */
            break;

        default:
            abort ();
        }
    }

    /* Instead of reporting ‘--verbose’
       and ‘--brief’ as they are encountered,
       we report the final status resulting from them. */
    if (verbose_flag) {
        puts ("verbose flag is set");
    }

		if (uwBaudRate != 9600 && uwBaudRate != 115200 ) {
        fprintf(stderr, "invalid baudrate, try 9600 or 115200 instead of %d\n", uwBaudRate);
        return -1;
    }

		if (uwVolt > 1900 ) {
        fprintf(stderr, "invalid voltage, try 500 for 5.00 volt %d\n", uwVolt);
        return -1;
    }
    printf("DPS bluetooth\n");
    mb = modbus_new_rtu("/dev/rfcomm0", uwBaudRate, 'N', 8, 1);
    if (mb == NULL) {
        fprintf(stderr, "Unable to create the libmodbus context\n");
        return -1;
    }
    modbus_connect(mb);

    modbus_set_slave(mb, 1);

    rc = modbus_read_registers(mb, 0x00, 10, tab_reg);
    if (rc == -1) {
        fprintf(stderr, "oops %s\n", modbus_strerror(errno));
        return -1;
    }
    for (i=0; i < rc; i++) {
        printf("reg[%d]=%d (0x%X)\n", i, tab_reg[i], tab_reg[i]);
    }

    if ( uwVolt > 0 ) {
      rc = modbus_write_register(mb, 0x000, uwVolt);
      if (rc == -1) {
          fprintf(stderr, "oops %s\n", modbus_strerror(errno));
          return -1;
      }
      for (i=0; i < rc; i++) {
          printf("reg[%d]=%d (0x%X)\n", i, tab_reg[i], tab_reg[i]);
      }

    }

    modbus_close(mb);
    modbus_free(mb);


    return 0;
}
