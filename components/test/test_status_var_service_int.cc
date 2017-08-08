/* Copyright (c) 2017, Oracle and/or its affiliates. All rights reserved.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; version 2 of the License.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA */

#include <mysql/components/component_implementation.h>
#include <mysql/components/service_implementation.h>
#include <mysql/components/services/component_status_var_service.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <typelib.h>

#define MAX_BUFFER_LENGTH 100
int log_text_len= 0;
char log_text[MAX_BUFFER_LENGTH];
FILE *outfile;
const char *filename= "test_component_status_var_service_int.log";

#define WRITE_LOG(format,lit_log_text) \
  log_text_len= sprintf(log_text, format, lit_log_text); \
  fwrite((uchar*)log_text, sizeof(char), log_text_len, outfile)

REQUIRES_SERVICE_PLACEHOLDER(status_variable_registration);

static int int_variable_value=4096;
static int int_var_2_value=2048;
static unsigned int uint_variable_value=0;

STATUS_VAR int_variable[]=
{ 
  {"test_int_component.int_variable", (char *) &int_variable_value,
   SHOW_INT, SHOW_SCOPE_GLOBAL},
  {0, 0, SHOW_UNDEF, SHOW_SCOPE_UNDEF} // null terminator required
};
STATUS_VAR int_var_2[]=
{ 
  {"int_var_2", (char *) &int_var_2_value,
   SHOW_INT, SHOW_SCOPE_GLOBAL},
  {0, 0, SHOW_UNDEF, SHOW_SCOPE_UNDEF} // null terminator required
};
STATUS_VAR uint_variable[]=
{ 
  {"test_int_component.uint_variable", (char *) &uint_variable_value,
   SHOW_INT, SHOW_SCOPE_GLOBAL},
  {0, 0, SHOW_UNDEF, SHOW_SCOPE_UNDEF} // null terminator required
};

/**
  Initialization entry method for test component. It executes the tests of
  the service.
*/
static mysql_service_status_t test_component_status_var_service_int_init()
{
  outfile= fopen(filename, "w+");

  WRITE_LOG("%s\n", "test_component_status_var_int init:");

  if (mysql_service_status_variable_registration->register_variable(
                 (STATUS_VAR *) &int_variable))
  {
    WRITE_LOG ("%s\n", "int register_variable failed.");
  }

  if (mysql_service_status_variable_registration->register_variable(
                 (STATUS_VAR *) &int_var_2))
  {
    WRITE_LOG ("%s\n", "int register_variable_2 failed.");
  }

  if (mysql_service_status_variable_registration->register_variable(
                 (STATUS_VAR *) &uint_variable))
  {
    WRITE_LOG ("%s\n", "uint register_variable failed.");
  }

  WRITE_LOG("%s\n", "test_component_status_var_int end of init:");
  fclose(outfile);

  return false;
}

/**
  De-initialization method for Component.
*/
static mysql_service_status_t test_component_status_var_service_int_deinit()
{

  outfile= fopen(filename, "a+");
  WRITE_LOG("%s\n", "test_component_status_var_int deinit:");

  if (mysql_service_status_variable_registration->unregister_variable(
                 (STATUS_VAR *) &int_variable))
  {
    WRITE_LOG ("%s\n", "int unregister_variable failed.");
  }

// Unregister status variable a second time
  if (mysql_service_status_variable_registration->unregister_variable(
                 (STATUS_VAR *) &int_variable))
  {
    WRITE_LOG ("%s\n", "int unregister_variable failed.");
  }

  if (mysql_service_status_variable_registration->unregister_variable(
                 (STATUS_VAR *) &int_var_2))
  {
    WRITE_LOG ("%s\n", "int unregister_variable failed.");
  }

  if (mysql_service_status_variable_registration->unregister_variable(
                 (STATUS_VAR *) &uint_variable))
  {
    WRITE_LOG ("%s\n", "uint unregister_variable failed.");
  }
  
  WRITE_LOG("%s\n", "test_component_status_var_int end of deinit:");

  fclose(outfile);
  return false;
}

/* An empty list as no service is provided. */
BEGIN_COMPONENT_PROVIDES(test_component_status_var_service)
END_COMPONENT_PROVIDES()

/* A list of required services. */
BEGIN_COMPONENT_REQUIRES(test_component_status_var_service)
  REQUIRES_SERVICE(status_variable_registration)
END_COMPONENT_REQUIRES()

/* A list of metadata to describe the Component. */
BEGIN_COMPONENT_METADATA(test_component_status_var_service)
  METADATA("mysql.author", "Oracle Corporation")
  METADATA("mysql.license", "GPL")
  METADATA("test_component_status_var_service", "1")
END_COMPONENT_METADATA()

/* Declaration of the Component. */
DECLARE_COMPONENT(test_component_status_var_service, "mysql:test_component_status_var_service")
  test_component_status_var_service_int_init,
  test_component_status_var_service_int_deinit
END_DECLARE_COMPONENT()

/* Defines list of Components contained in this library. Note that for now we
  assume that library will have exactly one Component. */
DECLARE_LIBRARY_COMPONENTS
  &COMPONENT_REF(test_component_status_var_service)
END_DECLARE_LIBRARY_COMPONENTS
