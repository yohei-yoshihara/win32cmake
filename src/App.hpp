#ifndef __APP__
#define __APP__

#include <UIRibbon.h>
#include <wrl/client.h>
#include "CommandHandler.hpp"

#define WM_RIBBON_RESIZED WM_USER + 1

namespace win32cmake {
  class App: public IUIApplication {
    LONG m_refCount = 0;
    HWND m_hWnd = 0;
    IUIFramework* m_pFramework = nullptr;
    Microsoft::WRL::ComPtr<CommandHandler> m_pCommandHandler = nullptr;
    Microsoft::WRL::ComPtr<IUIRibbon> m_pRibbon = nullptr;

  public:
    static HRESULT CreateInstance(App** ppApp);

    STDMETHODIMP_(ULONG) AddRef() override;
    STDMETHODIMP_(ULONG) Release() override;
    STDMETHODIMP QueryInterface(REFIID iid, void** ppv) override;

    // IUIApplication (start)
    IFACEMETHODIMP OnCreateUICommand(UINT32 nCmdID, UI_COMMANDTYPE typeID,
      IUICommandHandler** ppCommandHandler) override;
    IFACEMETHODIMP OnViewChanged(UINT32 nViewID, UI_VIEWTYPE typeID, IUnknown* pView, UI_VIEWVERB verb,
      INT32 uReasonCode) override;
    IFACEMETHODIMP OnDestroyUICommand(UINT32 commandId, UI_COMMANDTYPE typeID,
      IUICommandHandler* pCommandHandler) override;
    // IUIApplication (end)

    void SetHWnd(HWND hWnd) { m_hWnd = hWnd;  }
    HWND HWnd() const { return m_hWnd; }
  };
}

#endif