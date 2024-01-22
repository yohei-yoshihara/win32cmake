#include "CommandHandler.hpp"

namespace win32cmake {

  HRESULT CommandHandler::CreateInstance(CommandHandler** ppCommandHandler) {
    if (!ppCommandHandler) {
      return E_POINTER;
    }

    *ppCommandHandler = nullptr;
    HRESULT hr = S_OK;
    *ppCommandHandler = new CommandHandler();
    return hr;
  }

  STDMETHODIMP_(ULONG) CommandHandler::AddRef() {
    return InterlockedIncrement(&m_refCount);
  }

  STDMETHODIMP_(ULONG) CommandHandler::Release() {
    auto refCount = InterlockedDecrement(&m_refCount);
    if (refCount == 0) {
      delete this;
    }
    return refCount;
  }

  STDMETHODIMP CommandHandler::QueryInterface(REFIID iid, void** ppv) {
    if (iid == __uuidof(IUnknown)) {
      *ppv = static_cast<IUnknown*>(this);
    }
    else if (iid == __uuidof(IUICommandHandler)) {
      *ppv = static_cast<IUICommandHandler*>(this);
    }
    else {
      *ppv = nullptr;
      return E_NOINTERFACE;
    }

    AddRef();
    return S_OK;
  }

  STDMETHODIMP CommandHandler::UpdateProperty(
    UINT nCmdID,
    __in REFPROPERTYKEY key,
    __in_opt const PROPVARIANT* ppropvarCurrentValue,
    __out PROPVARIANT* ppropvarNewValue)
  {
    UNREFERENCED_PARAMETER(nCmdID);
    UNREFERENCED_PARAMETER(key);
    UNREFERENCED_PARAMETER(ppropvarCurrentValue);
    UNREFERENCED_PARAMETER(ppropvarNewValue);

    return E_NOTIMPL;
  }

  STDMETHODIMP CommandHandler::Execute(
    UINT nCmdID,
    UI_EXECUTIONVERB verb,
    __in_opt const PROPERTYKEY* key,
    __in_opt const PROPVARIANT* ppropvarValue,
    __in_opt IUISimplePropertySet* pCommandExecutionProperties)
  {
    UNREFERENCED_PARAMETER(pCommandExecutionProperties);
    UNREFERENCED_PARAMETER(ppropvarValue);
    UNREFERENCED_PARAMETER(key);
    UNREFERENCED_PARAMETER(verb);
    UNREFERENCED_PARAMETER(nCmdID);

    return S_OK;
  }

}

