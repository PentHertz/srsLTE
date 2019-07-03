/*
 * Copyright 2013-2019 Software Radio Systems Limited
 *
 * This file is part of srsLTE.
 *
 * srsLTE is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * srsLTE is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * A copy of the GNU Affero General Public License can be found in
 * the LICENSE file in the top-level directory of this distribution
 * and at http://www.gnu.org/licenses/.
 *
 */

#ifndef SRSLTE_PDCP_ENTITY_NR_H
#define SRSLTE_PDCP_ENTITY_NR_H

#include "srslte/common/buffer_pool.h"
#include "srslte/common/log.h"
#include "srslte/common/common.h"
#include "srslte/common/interfaces_common.h"
#include "srslte/common/security.h"
#include "srslte/common/threads.h"
#include "pdcp_entity_base.h"

namespace srslte {

/****************************************************************************
 * PDCP Entity interface
 * Common interface for all PDCP entities
 ***************************************************************************/
class pdcp_entity_nr : public pdcp_entity_base
{
public:
  pdcp_entity_nr();
  ~pdcp_entity_nr();
  void init(srsue::rlc_interface_pdcp* rlc_,
            srsue::rrc_interface_pdcp* rrc_,
            srsue::gw_interface_pdcp*  gw_,
            srslte::log*               log_,
            uint32_t                   lcid_,
            srslte_pdcp_config_nr_t    cfg_);
  void reset();
  void reestablish();

  // RRC interface
  void write_sdu(unique_byte_buffer_t sdu, bool blocking);
  void config_security(uint8_t*                    k_rrc_enc_,
                       uint8_t*                    k_rrc_int_,
                       uint8_t*                    k_up_enc_,
                       uint8_t*                    k_up_int_,
                       CIPHERING_ALGORITHM_ID_ENUM cipher_algo_,
                       INTEGRITY_ALGORITHM_ID_ENUM integ_algo_);
  void enable_integrity();
  void enable_encryption();

  uint32_t get_dl_count();
  uint32_t get_ul_count();

  // RLC interface
  void write_pdu(unique_byte_buffer_t pdu);

private:
  srsue::rlc_interface_pdcp* rlc = nullptr;
  srsue::rrc_interface_pdcp* rrc = nullptr;
  srsue::gw_interface_pdcp*  gw  = nullptr;

  srslte_pdcp_config_nr_t cfg;

  uint16_t sn_len_bytes = 0;

  // State variables: 3GPP TS 38.323 v15.2.0, section 7.1
  uint32_t tx_next = 0;
  uint32_t rx_next = 0;
  uint32_t rx_deliv = 0; 
  uint32_t rx_reord = 0; 
 
  // Constants: 3GPP TS 38.323 v15.2.0, section 7.2
  uint32_t window_size = 0;
};

/****************************************************************************
 * Pack/Unpack helper functions
 * Ref: 3GPP TS 38.323 v15.2.0
 ***************************************************************************/

} // namespace srslte
#endif // SRSLTE_PDCP_ENTITY_NR_H