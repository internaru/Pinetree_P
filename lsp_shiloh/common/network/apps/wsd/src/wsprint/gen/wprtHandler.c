/* wprtHandler.c
   Generated by gSOAP 2.7.8c from gen/WSDPrinterService.gsoap
   Copyright (C) 2000-2006, Robert van Engelen, Genivia Inc. All Rights Reserved.
   This part of the software is released under one of the following licenses:
   GPL, the gSOAP public license, or Genivia's license for commercial use.
*/
#include "wprtH.h"
#ifdef __cplusplus
extern "C" {
#endif

SOAP_SOURCE_STAMP("@(#) wprtHandler.c ver 2.7.8c 2013-09-16 07:14:30 GMT")


#ifndef WITH_NOSERVEREQUEST
SOAP_FMAC5 int SOAP_FMAC6 wprt_handle_event(struct dpws *dpws)
{
	struct soap *soap = &dpws->soap;
	const char* action = dpws->action ? dpws->action : "";
	dpws_set_namespaces(dpws, wprt_namespaces);
	soap_peek_element(soap);
	if (!strcmp(action, "http://schemas.microsoft.com/windows/2006/08/wdp/print/CreatePrintJobResponse") || !soap_match_tag(soap, soap->tag, "wprt:CreatePrintJobResponseType"))
		return soap_handle___wprt__CreatePrintJob(soap);
	if (!strcmp(action, "http://schemas.microsoft.com/windows/2006/08/wdp/print/SendDocumentResponse") || !soap_match_tag(soap, soap->tag, "wprt:SendDocumentResponseType"))
		return soap_handle___wprt__SendDocument(soap);
	if (!strcmp(action, "http://schemas.microsoft.com/windows/2006/08/wdp/print/AddDocumentResponse") || !soap_match_tag(soap, soap->tag, "wprt:AddDocumentResponseType"))
		return soap_handle___wprt__AddDocument(soap);
	if (!strcmp(action, "http://schemas.microsoft.com/windows/2006/08/wdp/print/CancelJobResponse") || !soap_match_tag(soap, soap->tag, "wprt:CancelJobResponseType"))
		return soap_handle___wprt__CancelJob(soap);
	if (!strcmp(action, "http://schemas.microsoft.com/windows/2006/08/wdp/print/GetPrinterElementsResponse") || !soap_match_tag(soap, soap->tag, "wprt:GetPrinterElementsResponseType"))
		return soap_handle___wprt__GetPrinterElements(soap);
	if (!strcmp(action, "http://schemas.microsoft.com/windows/2006/08/wdp/print/GetJobElementsResponse") || !soap_match_tag(soap, soap->tag, "wprt:GetJobElementsResponseType"))
		return soap_handle___wprt__GetJobElements(soap);
	if (!strcmp(action, "http://schemas.microsoft.com/windows/2006/08/wdp/print/GetActiveJobsResponse") || !soap_match_tag(soap, soap->tag, "wprt:GetActiveJobsResponseType"))
		return soap_handle___wprt__GetActiveJobs(soap);
	if (!strcmp(action, "http://schemas.microsoft.com/windows/2006/08/wdp/print/GetJobHistoryResponse") || !soap_match_tag(soap, soap->tag, "wprt:GetJobHistoryResponseType"))
		return soap_handle___wprt__GetJobHistory(soap);
	if (!strcmp(action, "http://schemas.microsoft.com/windows/2006/08/wdp/print/SetEventRateResponse") || !soap_match_tag(soap, soap->tag, "wprt:SetEventRateResponseType"))
		return soap_handle___wprt__SetEventRate(soap);
	if (!strcmp(action, "http://schemas.microsoft.com/windows/2006/08/wdp/print/PrinterElementsChangeEvent") || (!strlen(action) && !soap_match_tag(soap, soap->tag, "wprt:PrinterElementsChangeEvent")))
		return soap_serve___wprt__PrinterElementsChangeEvent(soap);
	if (!strcmp(action, "http://schemas.microsoft.com/windows/2006/08/wdp/print/PrinterStatusSummaryEvent") || (!strlen(action) && !soap_match_tag(soap, soap->tag, "wprt:PrinterStatusSummaryEvent")))
		return soap_serve___wprt__PrinterStatusSummaryEvent(soap);
	if (!strcmp(action, "http://schemas.microsoft.com/windows/2006/08/wdp/print/PrinterStatusConditionEvent") || (!strlen(action) && !soap_match_tag(soap, soap->tag, "wprt:PrinterStatusConditionEvent")))
		return soap_serve___wprt__PrinterStatusConditionEvent(soap);
	if (!strcmp(action, "http://schemas.microsoft.com/windows/2006/08/wdp/print/PrinterStatusConditionClearedEvent") || (!strlen(action) && !soap_match_tag(soap, soap->tag, "wprt:PrinterStatusConditionClearedEvent")))
		return soap_serve___wprt__PrinterStatusConditionClearedEvent(soap);
	if (!strcmp(action, "http://schemas.microsoft.com/windows/2006/08/wdp/print/JobStatusEvent") || (!strlen(action) && !soap_match_tag(soap, soap->tag, "wprt:JobStatusEvent")))
		return soap_serve___wprt__JobStatusEvent(soap);
	if (!strcmp(action, "http://schemas.microsoft.com/windows/2006/08/wdp/print/JobEndStateEvent") || (!strlen(action) && !soap_match_tag(soap, soap->tag, "wprt:JobEndStateEvent")))
		return soap_serve___wprt__JobEndStateEvent(soap);
	return soap->error = SOAP_NO_METHOD;
}
#endif

SOAP_FMAC5 int SOAP_FMAC6 soap_handle___wprt__CreatePrintJob(struct soap* soap)
{
	struct wprt__CreatePrintJobResponseType wprt__CreatePrintJobResponse;
	soap_default_wprt__CreatePrintJobResponseType(soap, &wprt__CreatePrintJobResponse);
	soap_get_wprt__CreatePrintJobResponseType(soap, &wprt__CreatePrintJobResponse, "wprt:CreatePrintJobResponseType", "wprt:CreatePrintJobResponseType");
	if (soap->error)
	{	if (soap->error == SOAP_TAG_MISMATCH && soap->level == 2)
			return soap_recv_fault(soap);
		return soap_closesock(soap);
	}
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	soap_closesock(soap);
	return __wprt__CreatePrintJob_handler(dpws_soap2dpws(soap), &wprt__CreatePrintJobResponse);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_handle___wprt__SendDocument(struct soap* soap)
{
	struct wprt__SendDocumentResponseType wprt__SendDocumentResponse;
	soap_default_wprt__SendDocumentResponseType(soap, &wprt__SendDocumentResponse);
	soap_get_wprt__SendDocumentResponseType(soap, &wprt__SendDocumentResponse, "wprt:SendDocumentResponseType", "wprt:SendDocumentResponseType");
	if (soap->error)
	{	if (soap->error == SOAP_TAG_MISMATCH && soap->level == 2)
			return soap_recv_fault(soap);
		return soap_closesock(soap);
	}
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	soap_closesock(soap);
	return __wprt__SendDocument_handler(dpws_soap2dpws(soap), &wprt__SendDocumentResponse);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_handle___wprt__AddDocument(struct soap* soap)
{
	struct wprt__AddDocumentResponseType wprt__AddDocumentResponse;
	soap_default_wprt__AddDocumentResponseType(soap, &wprt__AddDocumentResponse);
	soap_get_wprt__AddDocumentResponseType(soap, &wprt__AddDocumentResponse, "wprt:AddDocumentResponseType", "wprt:AddDocumentResponseType");
	if (soap->error)
	{	if (soap->error == SOAP_TAG_MISMATCH && soap->level == 2)
			return soap_recv_fault(soap);
		return soap_closesock(soap);
	}
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	soap_closesock(soap);
	return __wprt__AddDocument_handler(dpws_soap2dpws(soap), &wprt__AddDocumentResponse);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_handle___wprt__CancelJob(struct soap* soap)
{
	struct wprt__CancelJobResponseType wprt__CancelJobResponse;
	soap_default_wprt__CancelJobResponseType(soap, &wprt__CancelJobResponse);
	soap_get_wprt__CancelJobResponseType(soap, &wprt__CancelJobResponse, "wprt:CancelJobResponseType", "wprt:CancelJobResponseType");
	if (soap->error)
	{	if (soap->error == SOAP_TAG_MISMATCH && soap->level == 2)
			return soap_recv_fault(soap);
		return soap_closesock(soap);
	}
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	soap_closesock(soap);
	return __wprt__CancelJob_handler(dpws_soap2dpws(soap), &wprt__CancelJobResponse);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_handle___wprt__GetPrinterElements(struct soap* soap)
{
	struct wprt__GetPrinterElementsResponseType wprt__GetPrinterElementsResponse;
	soap_default_wprt__GetPrinterElementsResponseType(soap, &wprt__GetPrinterElementsResponse);
	soap_get_wprt__GetPrinterElementsResponseType(soap, &wprt__GetPrinterElementsResponse, "wprt:GetPrinterElementsResponseType", "wprt:GetPrinterElementsResponseType");
	if (soap->error)
	{	if (soap->error == SOAP_TAG_MISMATCH && soap->level == 2)
			return soap_recv_fault(soap);
		return soap_closesock(soap);
	}
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	soap_closesock(soap);
	return __wprt__GetPrinterElements_handler(dpws_soap2dpws(soap), &wprt__GetPrinterElementsResponse);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_handle___wprt__GetJobElements(struct soap* soap)
{
	struct wprt__GetJobElementsResponseType wprt__GetJobElementsResponse;
	soap_default_wprt__GetJobElementsResponseType(soap, &wprt__GetJobElementsResponse);
	soap_get_wprt__GetJobElementsResponseType(soap, &wprt__GetJobElementsResponse, "wprt:GetJobElementsResponseType", "wprt:GetJobElementsResponseType");
	if (soap->error)
	{	if (soap->error == SOAP_TAG_MISMATCH && soap->level == 2)
			return soap_recv_fault(soap);
		return soap_closesock(soap);
	}
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	soap_closesock(soap);
	return __wprt__GetJobElements_handler(dpws_soap2dpws(soap), &wprt__GetJobElementsResponse);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_handle___wprt__GetActiveJobs(struct soap* soap)
{
	struct wprt__GetActiveJobsResponseType wprt__GetActiveJobsResponse;
	soap_default_wprt__GetActiveJobsResponseType(soap, &wprt__GetActiveJobsResponse);
	soap_get_wprt__GetActiveJobsResponseType(soap, &wprt__GetActiveJobsResponse, "wprt:GetActiveJobsResponseType", "wprt:GetActiveJobsResponseType");
	if (soap->error)
	{	if (soap->error == SOAP_TAG_MISMATCH && soap->level == 2)
			return soap_recv_fault(soap);
		return soap_closesock(soap);
	}
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	soap_closesock(soap);
	return __wprt__GetActiveJobs_handler(dpws_soap2dpws(soap), &wprt__GetActiveJobsResponse);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_handle___wprt__GetJobHistory(struct soap* soap)
{
	struct wprt__GetJobHistoryResponseType wprt__GetJobHistoryResponse;
	soap_default_wprt__GetJobHistoryResponseType(soap, &wprt__GetJobHistoryResponse);
	soap_get_wprt__GetJobHistoryResponseType(soap, &wprt__GetJobHistoryResponse, "wprt:GetJobHistoryResponseType", "wprt:GetJobHistoryResponseType");
	if (soap->error)
	{	if (soap->error == SOAP_TAG_MISMATCH && soap->level == 2)
			return soap_recv_fault(soap);
		return soap_closesock(soap);
	}
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	soap_closesock(soap);
	return __wprt__GetJobHistory_handler(dpws_soap2dpws(soap), &wprt__GetJobHistoryResponse);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_handle___wprt__SetEventRate(struct soap* soap)
{
	struct wprt__SetEventRateResponseType wprt__SetEventRateResponse;
	soap_default_wprt__SetEventRateResponseType(soap, &wprt__SetEventRateResponse);
	soap_get_wprt__SetEventRateResponseType(soap, &wprt__SetEventRateResponse, "wprt:SetEventRateResponseType", "wprt:SetEventRateResponseType");
	if (soap->error)
	{	if (soap->error == SOAP_TAG_MISMATCH && soap->level == 2)
			return soap_recv_fault(soap);
		return soap_closesock(soap);
	}
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	soap_closesock(soap);
	return __wprt__SetEventRate_handler(dpws_soap2dpws(soap), &wprt__SetEventRateResponse);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_serve___wprt__PrinterElementsChangeEvent(struct soap *soap)
{	struct __wprt__PrinterElementsChangeEvent soap_tmp___wprt__PrinterElementsChangeEvent;
	soap_default___wprt__PrinterElementsChangeEvent(soap, &soap_tmp___wprt__PrinterElementsChangeEvent);
	if (dpws_enforce_mep(dpws_soap2dpws(soap), DC_TRUE))
		return soap->error;
	soap->encodingStyle = NULL;
	if (!soap_get___wprt__PrinterElementsChangeEvent(soap, &soap_tmp___wprt__PrinterElementsChangeEvent, "-wprt:PrinterElementsChangeEvent", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap)
)
		return soap->error;
	soap->error = __wprt__PrinterElementsChangeEvent(dpws_soap2dpws(soap), soap_tmp___wprt__PrinterElementsChangeEvent.wprt__PrinterElementsChangeEvent);
	if (soap->error)
		return soap->error;
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_serve___wprt__PrinterStatusSummaryEvent(struct soap *soap)
{	struct __wprt__PrinterStatusSummaryEvent soap_tmp___wprt__PrinterStatusSummaryEvent;
	soap_default___wprt__PrinterStatusSummaryEvent(soap, &soap_tmp___wprt__PrinterStatusSummaryEvent);
	if (dpws_enforce_mep(dpws_soap2dpws(soap), DC_TRUE))
		return soap->error;
	soap->encodingStyle = NULL;
	if (!soap_get___wprt__PrinterStatusSummaryEvent(soap, &soap_tmp___wprt__PrinterStatusSummaryEvent, "-wprt:PrinterStatusSummaryEvent", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap)
)
		return soap->error;
	soap->error = __wprt__PrinterStatusSummaryEvent(dpws_soap2dpws(soap), soap_tmp___wprt__PrinterStatusSummaryEvent.wprt__PrinterStatusSummaryEvent);
	if (soap->error)
		return soap->error;
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_serve___wprt__PrinterStatusConditionEvent(struct soap *soap)
{	struct __wprt__PrinterStatusConditionEvent soap_tmp___wprt__PrinterStatusConditionEvent;
	soap_default___wprt__PrinterStatusConditionEvent(soap, &soap_tmp___wprt__PrinterStatusConditionEvent);
	if (dpws_enforce_mep(dpws_soap2dpws(soap), DC_TRUE))
		return soap->error;
	soap->encodingStyle = NULL;
	if (!soap_get___wprt__PrinterStatusConditionEvent(soap, &soap_tmp___wprt__PrinterStatusConditionEvent, "-wprt:PrinterStatusConditionEvent", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap)
)
		return soap->error;
	soap->error = __wprt__PrinterStatusConditionEvent(dpws_soap2dpws(soap), soap_tmp___wprt__PrinterStatusConditionEvent.wprt__PrinterStatusConditionEvent);
	if (soap->error)
		return soap->error;
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_serve___wprt__PrinterStatusConditionClearedEvent(struct soap *soap)
{	struct __wprt__PrinterStatusConditionClearedEvent soap_tmp___wprt__PrinterStatusConditionClearedEvent;
	soap_default___wprt__PrinterStatusConditionClearedEvent(soap, &soap_tmp___wprt__PrinterStatusConditionClearedEvent);
	if (dpws_enforce_mep(dpws_soap2dpws(soap), DC_TRUE))
		return soap->error;
	soap->encodingStyle = NULL;
	if (!soap_get___wprt__PrinterStatusConditionClearedEvent(soap, &soap_tmp___wprt__PrinterStatusConditionClearedEvent, "-wprt:PrinterStatusConditionClearedEvent", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap)
)
		return soap->error;
	soap->error = __wprt__PrinterStatusConditionClearedEvent(dpws_soap2dpws(soap), soap_tmp___wprt__PrinterStatusConditionClearedEvent.wprt__PrinterStatusConditionClearedEvent);
	if (soap->error)
		return soap->error;
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_serve___wprt__JobStatusEvent(struct soap *soap)
{	struct __wprt__JobStatusEvent soap_tmp___wprt__JobStatusEvent;
	soap_default___wprt__JobStatusEvent(soap, &soap_tmp___wprt__JobStatusEvent);
	if (dpws_enforce_mep(dpws_soap2dpws(soap), DC_TRUE))
		return soap->error;
	soap->encodingStyle = NULL;
	if (!soap_get___wprt__JobStatusEvent(soap, &soap_tmp___wprt__JobStatusEvent, "-wprt:JobStatusEvent", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap)
)
		return soap->error;
	soap->error = __wprt__JobStatusEvent(dpws_soap2dpws(soap), soap_tmp___wprt__JobStatusEvent.wprt__JobStatusEvent);
	if (soap->error)
		return soap->error;
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_serve___wprt__JobEndStateEvent(struct soap *soap)
{	struct __wprt__JobEndStateEvent soap_tmp___wprt__JobEndStateEvent;
	soap_default___wprt__JobEndStateEvent(soap, &soap_tmp___wprt__JobEndStateEvent);
	if (dpws_enforce_mep(dpws_soap2dpws(soap), DC_TRUE))
		return soap->error;
	soap->encodingStyle = NULL;
	if (!soap_get___wprt__JobEndStateEvent(soap, &soap_tmp___wprt__JobEndStateEvent, "-wprt:JobEndStateEvent", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap)
)
		return soap->error;
	soap->error = __wprt__JobEndStateEvent(dpws_soap2dpws(soap), soap_tmp___wprt__JobEndStateEvent.wprt__JobEndStateEvent);
	if (soap->error)
		return soap->error;
	return soap_closesock(soap);
}

#ifdef __cplusplus
}
#endif

/* end of wprtHandler.c */