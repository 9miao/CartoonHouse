

#ifndef __CartoonHouse__DMDownloadBookInfo__
#define __CartoonHouse__DMDownloadBookInfo__

#include <stdio.h>
#include "CartoonHouse.h"

class DMDownloadBookInfo: public CAObject
{
    
public:
    
    CREATE_FUNC(DMDownloadBookInfo);
    
    std::vector<std::string> m_obDownloadPausesUrls;
    
    std::vector<std::string> m_obDownloadingUrls;
    
    std::vector<std::string> m_obWillDownloadUrls;
    
    std::vector<std::string> m_obDownloadFinishedListUrls;
    
    std::vector<std::string> m_obDownloadFinishedAloneUrls;
    
    std::map<std::string, DMIntroductionItemInfo> m_obListItemInfoMaps;
    
    std::map<std::string, DMIntroductionItemInfo> m_obAloneItemInfoMaps;
    
    std::map<std::string, unsigned long> m_obItemInfoIDMaps;
    
    std::string m_obFilePathTextTag;
    
    void sortPositivDownloadFinishUrls()
    {
        if (m_obDownloadFinishedListUrls.size() > 1)
        {
            for (int i=0; i<m_obDownloadFinishedListUrls.size()-1; i++)
            {
                for (int j=i+1; j<m_obDownloadFinishedListUrls.size(); j++)
                {
                    const DMIntroductionItemInfo& info1 = m_obListItemInfoMaps.at(m_obDownloadFinishedListUrls[i]);
                    const DMIntroductionItemInfo& info2 = m_obListItemInfoMaps.at(m_obDownloadFinishedListUrls[j]);
                    if (atoi(info1.chapter_order.c_str()) > atoi(info2.chapter_order.c_str()))
                    {
                        std::string tmp = m_obDownloadFinishedListUrls[i];
                        m_obDownloadFinishedListUrls[i] = m_obDownloadFinishedListUrls[j];
                        m_obDownloadFinishedListUrls[j] = tmp;
                    }
                }
            }
        }
        
        if (m_obDownloadFinishedAloneUrls.size() > 1)
        {
            for (int i=0; i<m_obDownloadFinishedAloneUrls.size()-1; i++)
            {
                for (int j=i+1; j<m_obDownloadFinishedAloneUrls.size(); j++)
                {
                    const DMIntroductionItemInfo& info1 = m_obListItemInfoMaps.at(m_obDownloadFinishedAloneUrls[i]);
                    const DMIntroductionItemInfo& info2 = m_obListItemInfoMaps.at(m_obDownloadFinishedAloneUrls[j]);
                    if (atoi(info1.chapter_order.c_str()) > atoi(info2.chapter_order.c_str()))
                    {
                        std::string tmp = m_obDownloadFinishedAloneUrls[i];
                        m_obDownloadFinishedAloneUrls[i] = m_obDownloadFinishedAloneUrls[j];
                        m_obDownloadFinishedAloneUrls[j] = tmp;
                    }
                }
            }
        }
    }
    
    void sortReverseDownloadFinishUrls()
    {
        if (m_obDownloadFinishedListUrls.size() > 1)
        {
            for (int i=0; i<m_obDownloadFinishedListUrls.size()-1; i++)
            {
                for (int j=i+1; j<m_obDownloadFinishedListUrls.size(); j++)
                {
                    const DMIntroductionItemInfo& info1 = m_obListItemInfoMaps.at(m_obDownloadFinishedListUrls[i]);
                    const DMIntroductionItemInfo& info2 = m_obListItemInfoMaps.at(m_obDownloadFinishedListUrls[j]);
                    if (atoi(info1.chapter_order.c_str()) < atoi(info2.chapter_order.c_str()))
                    {
                        std::string tmp = m_obDownloadFinishedListUrls[i];
                        m_obDownloadFinishedListUrls[i] = m_obDownloadFinishedListUrls[j];
                        m_obDownloadFinishedListUrls[j] = tmp;
                    }
                }
            }
        }
        
        if (m_obDownloadFinishedAloneUrls.size() > 1)
        {
            for (int i=0; i<m_obDownloadFinishedAloneUrls.size()-1; i++)
            {
                for (int j=i+1; j<m_obDownloadFinishedAloneUrls.size(); j++)
                {
                    const DMIntroductionItemInfo& info1 = m_obListItemInfoMaps.at(m_obDownloadFinishedAloneUrls[i]);
                    const DMIntroductionItemInfo& info2 = m_obListItemInfoMaps.at(m_obDownloadFinishedAloneUrls[j]);
                    if (atoi(info1.chapter_order.c_str()) < atoi(info2.chapter_order.c_str()))
                    {
                        std::string tmp = m_obDownloadFinishedAloneUrls[i];
                        m_obDownloadFinishedAloneUrls[i] = m_obDownloadFinishedAloneUrls[j];
                        m_obDownloadFinishedAloneUrls[j] = tmp;
                    }
                }
            }
        }
    }
    
    static std::string getDownloadUrl(const DMIntroductionInfo& info, const DMIntroductionItemInfo& itemInfo)
    {
        HTTP_URL::S_HTTP_URL s_http_url = HTTP_URL::get_HTTP_URL(HTTP_URL::HttpUrlTypeBookDownload, info.first_letter, info.ID, itemInfo.ID);
        return s_http_url.url + ".zip";
    }
    
    static std::string getFilePathTextTag(const DMIntroductionInfo& info)
    {
        return info.ID;
    }
};



#endif /* defined(__CartoonHouse__DMDownloadBookInfo__) */
