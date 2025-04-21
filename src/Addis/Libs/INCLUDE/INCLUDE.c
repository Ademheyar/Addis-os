/*
  * This file is part of Addis.
  *
  * Addis is free software: you can redistribute it and/or modify
  * it under the terms of the GNU General Public License as published by
  * the Free Software Foundation, either version 3 of the License, or
  * (at your option) any later version.
  *
  * Addis is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  * GNU General Public License for more details.
  *
  * You should have received a copy of the GNU General Public License
  * along with Addis.  If not, see <http://www.gnu.org/licenses/>.
  */
/*
  * @file INCLUDE.c
  * @brief This file contains the implementation of the INCLUDE keyword
  *        for reading and processing code in the Addis operating system.
  * @author Addis Team
  * @date 2023-10-01
  * @version 1.0
  * @note This file is part of the Addis operating system.
  *       It is licensed under the GNU General Public License v3.0.
  *       See the LICENSE file for more details.
  * @warning This file is for educational purposes only.
  *          Use at your own risk. The authors are not responsible for any damages
  *          caused by the use of this code.
*/
#include <Addis/Read_Do.h>

// Function to read and process the INCLUDE keyword
// This function is responsible for including files and reading their content.
// It handles both string and path formats for the included files.
// The function also manages the reading state and updates the task list accordingly.
// It is important to note that this function is part of a larger system
// and relies on other components for its functionality.
// The function is designed to be flexible and extensible,
// allowing for future enhancements and modifications.
// It is recommended to follow best practices for coding and documentation
struct ROW *READ_INCLUDE(list_t *read)
{
  DEBUG(" In Addis Code File Reading INCLUED Key Word Starting\n");
  // and get the next word After the INCLUED Key Word
  listnode_t *readword = list_get_node_by_index(read, task_list_current->holded_info->read->fread->reading_on);
  // chack if the next word is given
  if(!readword || !readword->value || !issame(readword->value, "")) {
    // chack if the next word is a string or path
    char *path = readword->value;
    // Let see if the path is a string or path By Printing it
    DEBUG(" Next Key Word In reading INCLUED KEY WORD IS (%s)\n", path);
    // chack if the path has path '\' in the meddle
    // or if it is a string
    if (strchr(path, '\\') || strchr(path, '/')) {
      // this will be used to include the file that are found in path form
      // and read it
      char *inctext = get_code(path);
      if (strlen(inctext) > 0) {
        DEBUG(" old codecode(%s)\n", inctext);
        task_list_current->holded_info->fread->state = 'L';
        char *incode = read_bodys(inctext);
        task_list_current->holded_info->read->fread->reading_value = str_splitL(incode, " ", 0);
        task_list_current->holded_info->read->fread->read_new = incode;
        //
        DEBUG(" done INCLUDing code(%s)\n", incode);
      }
    }
    else {
      // this will be used to include the file that are found in string form
      // and read it
      DEBUG(" include string c code(%s)\n", path);
    }
  }
  return 0;
}

