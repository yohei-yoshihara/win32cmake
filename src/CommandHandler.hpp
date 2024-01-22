#ifndef __CommandHandler_hpp__
#define __CommandHandler_hpp__

#include <UIRibbon.h>

namespace win32cmake {

  class CommandHandler : public IUICommandHandler {
    LONG m_refCount = 0;

  public:
    static HRESULT CreateInstance(CommandHandler** ppCommandHandler);

    // IUnknown interface
    IFACEMETHODIMP_(ULONG) AddRef() override;
    IFACEMETHODIMP_(ULONG) Release() override;
    IFACEMETHODIMP QueryInterface(REFIID riid, void** ppInterface) override;

    // IUICommandHandler methods
    STDMETHOD(UpdateProperty)(UINT nCmdID,
      __in REFPROPERTYKEY key,
      __in_opt const PROPVARIANT* ppropvarCurrentValue,
      __out PROPVARIANT* ppropvarNewValue) override;

    STDMETHOD(Execute)(UINT nCmdID,
      UI_EXECUTIONVERB verb,
      __in_opt const PROPERTYKEY* key,
      __in_opt const PROPVARIANT* ppropvarValue,
      __in_opt IUISimplePropertySet* pCommandExecutionProperties) override;

  };

}

#endif