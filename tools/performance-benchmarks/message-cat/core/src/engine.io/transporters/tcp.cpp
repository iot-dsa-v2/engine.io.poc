#include "stdafx.h"
#include "tcp.h"

TransporterTCP::TransporterTCP()
{
}

TransporterTCP::~TransporterTCP()
{
}

void TransporterTCP::enableTCPClient()
{
	tcpIOMonitor(UV_TCP);
	tcpIOMonitor.establishConnection();
}