#include "framework.h"
#include "WindowsProject1.h"
#include "tinyxml2.h"
#include <iostream>
#include <memory>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <UIRibbon.h>
#include <wrl/client.h>
#include "App.hpp"

using Microsoft::WRL::ComPtr;
using namespace win32cmake;
namespace xml = tinyxml2;

namespace win32cmake {
  std::shared_ptr<spdlog::logger> logger = spdlog::basic_logger_mt("basic_logger", "win32_cmake.log");

void test()
{
  spdlog::set_level(spdlog::level::debug);

  xml::XMLDocument doc;
  doc.Parse(
      "<element>a</element>");
  const char *text = doc.FirstChildElement("element")->GetText();
  logger->debug("text = {}", text);
}
}

#define MAX_LOADSTRING 100

// グローバル変数:
HINSTANCE hInst;                     // 現在のインターフェイス
WCHAR szTitle[MAX_LOADSTRING];       // タイトル バーのテキスト
WCHAR szWindowClass[MAX_LOADSTRING]; // メイン ウィンドウ クラス名

// このコード モジュールに含まれる関数の宣言を転送します:
ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance,
                      _In_ LPWSTR lpCmdLine,
                      _In_ int nCmdShow)
{
  UNREFERENCED_PARAMETER(hPrevInstance);
  UNREFERENCED_PARAMETER(lpCmdLine);

  HRESULT hr = CoInitialize(NULL);
  if (FAILED(hr))
  {
    return FALSE;
  }

  // TODO: ここにコードを挿入してください。
  win32cmake::test();

  // グローバル文字列を初期化する
  LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
  LoadStringW(hInstance, IDC_WINDOWSPROJECT1, szWindowClass, MAX_LOADSTRING);
  MyRegisterClass(hInstance);

  // アプリケーション初期化の実行:
  if (!InitInstance(hInstance, nCmdShow))
  {
    return FALSE;
  }

  HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSPROJECT1));

  MSG msg;

  // メイン メッセージ ループ:
  while (GetMessage(&msg, nullptr, 0, 0))
  {
    if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
    {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
  }

  return (int)msg.wParam;
}

//
//  関数: MyRegisterClass()
//
//  目的: ウィンドウ クラスを登録します。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
  WNDCLASSEXW wcex;

  wcex.cbSize = sizeof(WNDCLASSEX);

  wcex.style = 0; // CS_HREDRAW | CS_VREDRAW;
  wcex.lpfnWndProc = WndProc;
  wcex.cbClsExtra = 0;
  wcex.cbWndExtra = 0;
  wcex.hInstance = hInstance;
  wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSPROJECT1));
  wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
  wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
  wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_WINDOWSPROJECT1);
  wcex.lpszClassName = szWindowClass;
  wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

  return RegisterClassExW(&wcex);
}

IUIFramework *pFramework;

//
//   関数: InitInstance(HINSTANCE, int)
//
//   目的: インスタンス ハンドルを保存して、メイン ウィンドウを作成します
//
//   コメント:
//
//        この関数で、グローバル変数でインスタンス ハンドルを保存し、
//        メイン プログラム ウィンドウを作成および表示します。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
  hInst = hInstance; // グローバル変数にインスタンス ハンドルを格納する

  HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
                            CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

  if (!hWnd)
  {
    return FALSE;
  }

  ShowWindow(hWnd, nCmdShow);
  UpdateWindow(hWnd);

  return TRUE;
}

IUIFramework *g_pFramework = nullptr;
ComPtr<App> g_app = nullptr;

bool InitializeFramework(HWND hwnd)
{
  // Here we instantiate the Ribbon framework object.
  auto hr = CoCreateInstance(CLSID_UIRibbonFramework, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&g_pFramework));
  if (FAILED(hr)) {
    SPDLOG_LOGGER_ERROR(logger, "failed to create ribbon framework, hr = {}", hr);
    return false;
  }

  hr = App::CreateInstance(&g_app);
  if (FAILED(hr)) {
    SPDLOG_LOGGER_ERROR(logger, "failed to create App, hr = {}", hr);
    return false;
  }
  g_app->SetHWnd(hwnd);

  hr = g_pFramework->Initialize(hwnd, static_cast<IUIApplication *>(g_app.Get()));
  if (FAILED(hr)) {
    SPDLOG_LOGGER_ERROR(logger, "failed to initialize Ribbon Framework, hr = {}", hr);
    return false;
  }

  hr = g_pFramework->LoadUI(GetModuleHandle(NULL), L"APPLICATION_RIBBON");
  if (FAILED(hr)) {
    SPDLOG_LOGGER_ERROR(logger, "failed LoadUI, hr = {}", hr);
    return false;
  }

  return true;
}

//
//  関数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的: メイン ウィンドウのメッセージを処理します。
//
//  WM_COMMAND  - アプリケーション メニューの処理
//  WM_PAINT    - メイン ウィンドウを描画する
//  WM_DESTROY  - 中止メッセージを表示して戻る
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  switch (message)
  {
  case WM_CREATE:
  {
    auto b = InitializeFramework(hWnd);
    if (!b)
    {
      return -1;
    }
  }
  break;
  case WM_COMMAND:
  {
    int wmId = LOWORD(wParam);
    // 選択されたメニューの解析:
    switch (wmId)
    {
    case IDM_ABOUT:
      DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
      break;
    case IDM_EXIT:
      DestroyWindow(hWnd);
      break;
    default:
      return DefWindowProc(hWnd, message, wParam, lParam);
    }
  }
  break;
  case WM_PAINT:
  {
    //PAINTSTRUCT ps;
    //HDC hdc = BeginPaint(hWnd, &ps);
    // TODO: HDC を使用する描画コードをここに追加してください...
    //EndPaint(hWnd, &ps);
  }
  break;
  case WM_DESTROY:
    PostQuitMessage(0);
    break;
  default:
    return DefWindowProc(hWnd, message, wParam, lParam);
  }
  return 0;
}

// バージョン情報ボックスのメッセージ ハンドラーです。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
  UNREFERENCED_PARAMETER(lParam);
  switch (message)
  {
  case WM_INITDIALOG:
    return (INT_PTR)TRUE;

  case WM_COMMAND:
    if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
      EndDialog(hDlg, LOWORD(wParam));
      return (INT_PTR)TRUE;
    }
    break;
  }
  return (INT_PTR)FALSE;
}
