1. ip_rcv
linux-net-kernel\net\ipv4\ip_input.c

/********************************************************
 * 	Main IP Receive routine.
 * @skb: 接收到的IP数据包
 * @dev: 接收到的IP数据包当前的输入网络设备
 * @pt:输入此数据包的网络层输入接口
 * @orig_dev:接收到的IP数据包原始的输入网络设备。
 *********************************************************/
 //在data指针移动size(iphdr)后，移动到指向传输层的函数是ip_local_deliver_finish
 //如果有发送到本地的数据包，本地收到后可能需要从组，在函数ip_local_deliver中从组
int ip_rcv(struct sk_buff *skb, struct net_device *dev, struct packet_type *pt, struct net_device *orig_dev)
{
	struct iphdr *iph;//定义一个ip报文的数据报头
	u32 len;

	if((0 == dev_d1300) || (dev->name[3] != '2')) {
		/* When the interface is in promisc. mode, drop all the crap
		 * that it receives, do not try to analyse it.
		 */
		if (skb->pkt_type == PACKET_OTHERHOST) //数据包不是发给我们的,这里所说的“不属于”这个主机，是指在这个包目标主机的MAC地址不是本机，而不是L3层的ip地址。
			goto drop;
	}


	IP_UPD_PO_STATS_BH(dev_net(dev), IPSTATS_MIB_IN, skb->len); 

	if ((skb = skb_share_check(skb, GFP_ATOMIC)) == NULL) {//接下来是一个共享的检查，如果是共享的数据包，因为它可能需要修改skb中的信息，所以要先复制一个副本，再作进一步的处理。
		IP_INC_STATS_BH(dev_net(dev), IPSTATS_MIB_INDISCARDS);
		goto out;
	}

	if (!pskb_may_pull(skb, sizeof(struct iphdr)))//再下来就是检查首部的长度是否够长，校检和等等：
		goto inhdr_error;

	iph = ip_hdr(skb);

	/*
	 *	RFC1122: 3.2.1.2 MUST silently discard any IP frame that fails the checksum.
	 *
	 *	Is the datagram acceptable?
	 *
	 *	1.	Length at least the size of an ip header
	 *	2.	Version of 4
	 *	3.	Checksums correctly. [Speed optimisation for later, skip loopback checksums]
	 *	4.	Doesn't have a bogus length
	 */

	if (iph->ihl < 5 || iph->version != 4)
		goto inhdr_error;
/*
//iph->ihl<5说明iph->ihl指的是IP包的首部长度，首部一行是32bit也就是4byte（字节）注：1byte=8bit，byte是计算机中最小文件
单位，普通IP数据包首部长度（不包含任何选项）字段的值是5.*/
	if (!pskb_may_pull(skb, iph->ihl*4))//对数据报的头长度进行检查  //iph->ihl*4是20，是首部最长的长度,此语句是说如果头部长度不能pull，则error
		goto inhdr_error;

	iph = ip_hdr(skb);

	if (unlikely(ip_fast_csum((u8 *)iph, iph->ihl)))
		goto inhdr_error;

	len = ntohs(iph->tot_len);
	if (skb->len < len) {
		IP_INC_STATS_BH(dev_net(dev), IPSTATS_MIB_INTRUNCATEDPKTS);
		goto drop;
	} else if (len < (iph->ihl*4))
		goto inhdr_error;

	/* Our transport medium may have padded the buffer out. Now we know it
	 * is IP we can trim to the true length of the frame.
	 * Note this now means skb->len holds ntohs(iph->tot_len).
	 */
	if (pskb_trim_rcsum(skb, len)) {//根据ip包总长度，重新计算skb的长度，去掉末尾的无用信息
		IP_INC_STATS_BH(dev_net(dev), IPSTATS_MIB_INDISCARDS);
		goto drop;
	}

	/* Remove any debris in the socket control block */
	memset(IPCB(skb), 0, sizeof(struct inet_skb_parm));//这里面后面会存ip填充信息，IP如果超过20字节，就有填充信息

	/* Must drop socket now because of tproxy. */
	/*
	 * 将skb中的IP控制块清零，以便
	 * 后续对IP选项的处理
	 */
	skb_orphan(skb);

    /*
         * 最后通过netfilter模块处理后，调用ip_rcv_finish()
         * 完成IP数据包的输入。
         */
	return NF_HOOK(NFPROTO_IPV4, NF_INET_PRE_ROUTING, skb, dev, NULL,
		       ip_rcv_finish); //hook注册地方在nf_register_hooks

inhdr_error:
	IP_INC_STATS_BH(dev_net(dev), IPSTATS_MIB_INHDRERRORS);
drop:
	kfree_skb(skb);
out:
	return NET_RX_DROP;
}
