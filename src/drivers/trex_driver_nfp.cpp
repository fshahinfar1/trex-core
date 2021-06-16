/*
 * Farbod Shahinfar
 * June 2021
 */

#include "trex_driver_nfp.h"
#include "trex_driver_defines.h"

// std::string CTRexExtendedDriverBaseNFP::mlx5_so_str = "";

CTRexExtendedDriverBaseNFP::CTRexExtendedDriverBaseNFP()
{
    // m_cap = tdCAP_MULTI_QUE | tdCAP_ONE_QUE  | TREX_DRV_CAP_MAC_ADDR_CHG ;
    // m_cap = tdCAP_ALL  | TREX_DRV_CAP_MAC_ADDR_CHG ;
    m_cap = tdCAP_MULTI_QUE | tdCAP_ONE_QUE;
    for (int i=0; i<TREX_MAX_PORTS; i++ ) {
        m_port_xstats[i] = {0};
    }
}

static int _LinkStateChangedCB (uint16_t port_id, enum rte_eth_event_type event, void *cb_arg, void *ret_param)
{
	printf("link state changed callback\n");
	return 0;
}

TRexPortAttr* CTRexExtendedDriverBaseNFP::create_port_attr(tvpid_t tvpid,repid_t repid) {
	rte_eth_dev_callback_register(repid, RTE_ETH_EVENT_INTR_LSC, _LinkStateChangedCB, nullptr);
    return new DpdkTRexPortAttr(tvpid, repid, false, false, true, false, true, true);
}

// std::string& get_mlx5_so_string(void) {
//     return CTRexExtendedDriverBaseNFP::mlx5_so_str;
// }

bool CTRexExtendedDriverBaseNFP::is_support_for_rx_scatter_gather(){
    return false;
    // if (get_is_tcp_mode()) {
    //     return (false);
    // }
    // return (true);
}


int CTRexExtendedDriverBaseNFP::get_min_sample_rate(void){
    return (RX_CHECK_MIX_SAMPLE_RATE);
}


void CTRexExtendedDriverBaseNFP::clear_extended_stats(CPhyEthIF * _if){
    repid_t repid = _if->get_repid();
    rte_eth_stats_reset(repid);
}

void CTRexExtendedDriverBaseNFP::update_configuration(port_cfg_t * cfg){
    cfg->m_tx_conf.tx_thresh.pthresh = TX_PTHRESH;
    cfg->m_tx_conf.tx_thresh.hthresh = TX_HTHRESH;
    cfg->m_tx_conf.tx_thresh.wthresh = TX_WTHRESH;
    cfg->m_port_conf.fdir_conf.mode = RTE_FDIR_MODE_PERFECT;
    cfg->m_port_conf.fdir_conf.pballoc = RTE_FDIR_PBALLOC_64K;
    cfg->m_port_conf.fdir_conf.status = RTE_FDIR_NO_REPORT_STATUS;

    cfg->m_port_conf.rxmode.offloads = DEV_RX_OFFLOAD_JUMBO_FRAME;
}

void CTRexExtendedDriverBaseNFP::reset_rx_stats(CPhyEthIF * _if,
                uint32_t *stats, int min, int len) {
    for (int i =0; i < len; i++) {
        stats[i] = 0;
    }
}

int CTRexExtendedDriverBaseNFP::get_rx_stats(CPhyEthIF * _if, uint32_t *pkts,
                uint32_t *prev_pkts, uint32_t *bytes, uint32_t *prev_bytes,
                int min, int max) {
    /* not supported yet */
    return 0;
}

int CTRexExtendedDriverBaseNFP::dump_fdir_global_stats(CPhyEthIF * _if, FILE *fd)
{
    return 0;
}

bool CTRexExtendedDriverBaseNFP::get_extended_stats(CPhyEthIF * _if, CPhyEthIFStats *stats) {
    return get_extended_stats_fixed(_if, stats, 0, 0);
    // enum {
    //     rx_good_packets,
    //     tx_good_packets,
    //     rx_good_bytes,
    //     tx_good_bytes,
    //     rx_missed_errors,
    //     rx_errors,
    //     tx_errors,
    //     rx_mbuf_allocation_errors,
    //     COUNT
    // };

    // enum {
    //     rx_wqe_err,
    //     rx_port_unicast_packets,
    //     rx_port_unicast_bytes,
    //     tx_port_unicast_packets,
    //     tx_port_unicast_bytes,
    //     rx_port_multicast_packets,
    //     rx_port_multicast_bytes,
    //     tx_port_multicast_packets,
    //     tx_port_multicast_bytes,
    //     rx_port_broadcast_packets,
    //     rx_port_broadcast_bytes,
    //     tx_port_broadcast_packets,
    //     tx_port_broadcast_bytes,
    //     tx_packets_phy,
    //     rx_packets_phy,
    //     rx_crc_errors_phy,
    //     tx_bytes_phy,
    //     rx_bytes_phy,
    //     rx_in_range_len_errors_phy,
    //     rx_symbol_err_phy,
    //     rx_discards_phy,
    //     tx_discards_phy,
    //     tx_errors_phy,
    //     rx_out_of_buffer,
    //     //tx_pp_missed_interrupt_error,
    //     //tx_pp_rearm_queue_errors,
    //     //tx_pp_clock_queue_errors,    
    //     //tx_pp_timestamp_past_errors,
    //     //tx_pp_timestamp_future_error, 
    //     //tx_pp_jitter,
    //     //tx_pp_wander,
    //     //tx_pp_sync_lost,
    //     XCOUNT
    // };

    // uint16_t repid = _if->get_repid();
    // xstats_struct* xstats_struct = &m_port_xstats[repid];




    // if ( !xstats_struct->init ) {
    //     // total_count = COUNT + per queue stats count + XCOUNT
    //     xstats_struct->total_count = rte_eth_xstats_get(repid, NULL, 0);
    //     assert(xstats_struct->total_count>=COUNT+XCOUNT);
    //     struct rte_eth_xstat_name *xstats_names;

    //     xstats_names = (struct rte_eth_xstat_name *)malloc(sizeof(struct rte_eth_xstat_name) * xstats_struct->total_count);
    //     assert(xstats_names!= 0);
    //     rte_eth_xstats_get_names(repid,xstats_names,xstats_struct->total_count);
    //     int i;

    //     for (i=0; i<xstats_struct->total_count; i++){
    //         if (strncmp(xstats_names[i].name,"rx_out_of_buffer",RTE_ETH_XSTATS_NAME_SIZE) == 0) { 
    //             xstats_struct->last_offset = xstats_struct->total_count - 1 - i;
    //             break;
    //         }
    //     }
    //     free(xstats_names);
    // }

    // struct rte_eth_xstat xstats_array[xstats_struct->total_count];
    // struct rte_eth_xstat *xstats = &xstats_array[xstats_struct->total_count - XCOUNT - xstats_struct->last_offset];
    // struct rte_eth_stats *prev_stats = &stats->m_prev_stats;

    // /* fetch stats */
    // int ret;
    // ret = rte_eth_xstats_get(repid, xstats_array, xstats_struct->total_count);
    // assert(ret==xstats_struct->total_count);

    // uint32_t opackets = xstats[tx_port_unicast_packets].value +
    //                     xstats[tx_port_multicast_packets].value +
    //                     xstats[tx_port_broadcast_packets].value;
    // uint32_t ipackets = xstats[rx_port_unicast_packets].value +
    //                     xstats[rx_port_multicast_packets].value +
    //                     xstats[rx_port_broadcast_packets].value;
    // uint64_t obytes = xstats[tx_port_unicast_bytes].value +
    //                   xstats[tx_port_multicast_bytes].value +
    //                   xstats[tx_port_broadcast_bytes].value;
    // uint64_t ibytes = xstats[rx_port_unicast_bytes].value +
    //                   xstats[rx_port_multicast_bytes].value +
    //                   xstats[rx_port_broadcast_bytes].value;
    // uint64_t &imissed = xstats_array[rx_missed_errors].value;
    // uint64_t &rx_nombuf = xstats_array[rx_mbuf_allocation_errors].value;
    // uint64_t ierrors = xstats[rx_wqe_err].value +
    //                    xstats[rx_crc_errors_phy].value +
    //                    xstats[rx_in_range_len_errors_phy].value +
    //                    xstats[rx_symbol_err_phy].value +
    //                    xstats[rx_out_of_buffer].value;
    // uint64_t &oerrors = xstats[tx_errors_phy].value;

    // if ( !xstats_struct->init ) {
    //     xstats_struct->init = true;
    // } else {
    //     // Packet counter on Connect5 is 40 bits, use 32 bit diffs
    //     uint32_t packet_diff;
    //     packet_diff = opackets - (uint32_t)prev_stats->opackets;
    //     stats->opackets += packet_diff;
    //     stats->obytes += obytes - prev_stats->obytes +
    //                      packet_diff * 4; // add FCS

    //     packet_diff = ipackets - (uint32_t)prev_stats->ipackets;
    //     stats->ipackets += packet_diff;
    //     stats->ibytes += ibytes - prev_stats->ibytes +
    //                      packet_diff * 4; // add FCS

    //     stats->ierrors += imissed - prev_stats->imissed +
    //                       rx_nombuf - prev_stats->rx_nombuf +
    //                       ierrors - prev_stats->ierrors;
    //     stats->rx_nombuf += rx_nombuf - prev_stats->rx_nombuf;
    //     stats->oerrors += oerrors - prev_stats->oerrors;
    // }

    // prev_stats->ipackets  = ipackets;
    // prev_stats->opackets  = opackets;
    // prev_stats->ibytes    = ibytes;
    // prev_stats->obytes    = obytes;
    // prev_stats->imissed   = imissed;
    // prev_stats->rx_nombuf = rx_nombuf;
    // prev_stats->ierrors   = ierrors;
    // prev_stats->oerrors   = oerrors;

    // return true;
}

int CTRexExtendedDriverBaseNFP::wait_for_stable_link(){
	int rc = 0;
	struct rte_eth_link link = {};
	// rc = rte_eth_dev_start(0);
	// printf("start rc: %d\n", rc);
	for (i = 0; i < 5; i++) {
		delay(1000);
		rc = rte_eth_link_get(0, &link);
		if (link.link_status == ETH_LINK_DOWN) {
			printf("link down (%d)\n", rc);
		} else {
			printf("link up\n");
			return 0;
		}
	}
    return 1;
}

CFlowStatParser *CTRexExtendedDriverBaseNFP::get_flow_stat_parser() {
    CFlowStatParser *parser = new CFlowStatParser(CFlowStatParser::FLOW_STAT_PARSER_MODE_HW);
    assert (parser);
    return parser;
}

void CTRexExtendedDriverBaseNFP::get_rx_stat_capabilities(uint16_t &flags, uint16_t &num_counters, uint16_t &base_ip_id) {
    flags = TrexPlatformApi::IF_STAT_IPV4_ID | TrexPlatformApi::IF_STAT_RX_BYTES_COUNT
        | TrexPlatformApi::IF_STAT_PAYLOAD;
    num_counters = 127; //With MAX_FLOW_STATS we saw packet failures in rx_test. Need to check.
    base_ip_id = IP_ID_RESERVE_BASE;
}

