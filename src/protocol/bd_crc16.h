/* Copyright (c) [2014 Baidu]. All Rights Reserved.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * File Name          :
 * Author             :
 * Version            : $Revision:$
 * Date               : $Date:$
 * Description        :
 *
 * HISTORY:
 * Date               | Modification                    | Author
 * 28/03/2014         | Initial Revision                |

 */
#ifndef __CRC16_H
#define __CRC16_H
#include <stdint.h>

extern uint16_t const crc16_table[256];

extern uint16_t bd_crc16(uint16_t crc, const uint8_t *buffer, uint16_t len);

#endif /* __CRC16_H */
