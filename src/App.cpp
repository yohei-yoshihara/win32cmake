#include "App.hpp"
#include "Main.hpp"
#include "ribbonres.h"
#include <d3d12.h> 
#include <dxgi1_6.h>

using Microsoft::WRL::ComPtr;

namespace win32cmake {
  HRESULT App::CreateInstance(App** ppApp) {
    if (!ppApp) {
      return E_POINTER;
    }
    *ppApp = new App();
    return S_OK;
  }

  STDMETHODIMP_(ULONG) App::AddRef() {
    return InterlockedIncrement(&m_refCount);
  }

  STDMETHODIMP_(ULONG) App::Release() {
    LONG refCount = InterlockedDecrement(&m_refCount);
    if (refCount == 0) {
      delete this;
    }
    return refCount;
  }

  STDMETHODIMP_(HRESULT) App::QueryInterface(REFIID iid, void** ppv) {
    if (iid == __uuidof(IUnknown)) {
      *ppv = static_cast<IUnknown*>(this);
    }
    else if (iid == __uuidof(IUIApplication)) {
      *ppv = static_cast<IUIApplication*>(this);
    }
    else {
      *ppv = NULL;
      return E_NOINTERFACE;
    }
    AddRef();
    return S_OK;
  }

  // IUIApplication (start)
  /**
   * Called for each Command specified in the Windows Ribbon framework markup to bind the Command
   * to an IUICommandHandler.
   */
  HRESULT App::OnCreateUICommand(UINT32 nCmdID, UI_COMMANDTYPE typeID,
    IUICommandHandler** ppCommandHandler) {

    UNREFERENCED_PARAMETER(nCmdID);
    UNREFERENCED_PARAMETER(typeID);

    SPDLOG_LOGGER_TRACE(logger, "nCmdID = {}", nCmdID);
    HRESULT hr = S_OK;

    if (m_pCommandHandler == nullptr) {
      hr = CommandHandler::CreateInstance(m_pCommandHandler.ReleaseAndGetAddressOf());
      if (FAILED(hr)) {
        return hr;
      }
    }

    return m_pCommandHandler->QueryInterface(IID_PPV_ARGS(ppCommandHandler));
  }

  /**
   * Called when the state of a View changes.
   */
  HRESULT App::OnViewChanged(UINT32 nViewID, UI_VIEWTYPE typeID, IUnknown* pView,
    UI_VIEWVERB verb, INT32 uReasonCode) {
    UNREFERENCED_PARAMETER(nViewID);
    UNREFERENCED_PARAMETER(uReasonCode);

    HRESULT hr = E_NOTIMPL;
    if (UI_VIEWTYPE_RIBBON == typeID) {
      switch (verb) {
      case UI_VIEWVERB_CREATE:
        if (m_pRibbon == nullptr) {
          hr = pView->QueryInterface(m_pRibbon.ReleaseAndGetAddressOf());
        }
        break;
      case UI_VIEWVERB_SIZE:
        if (m_pRibbon != nullptr) {
          UINT32 height = 0;
          hr = m_pRibbon->GetHeight(&height);
          PostMessage(m_hWnd, WM_RIBBON_RESIZED, static_cast<WPARAM>(height), 0);
        }
        break;
      case UI_VIEWVERB_DESTROY:
        m_pRibbon = nullptr;
        hr = S_OK;
        break;
      }
    }
    return hr;
  }

  /**
   * Called for each Command specified in the Windows Ribbon framework markup
   * when the application window is destroyed.
   */
  HRESULT App::OnDestroyUICommand(UINT32 commandId, UI_COMMANDTYPE typeID,
    IUICommandHandler* pCommandHandler) {
    UNREFERENCED_PARAMETER(commandId);
    UNREFERENCED_PARAMETER(typeID);
    UNREFERENCED_PARAMETER(pCommandHandler);

    return E_NOTIMPL;
  }
  // IUIApplication (end)

}
