#include "Visual.h"
#include <Windows.h>

using namespace System;
using namespace System::Windows::Forms;
using namespace vebclr;


[STAThread]
//void main(array<String^>^ arg) {
int main() {
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);
    vebclr::Visual^ form = gcnew Visual();
    form->tr = new Veb(32);
    Application::Run(form);
    return 0;
}



//
//
//int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
//    Application::EnableVisualStyles();
//    Application::SetCompatibleTextRenderingDefault(false);
//    Application::Run(gcnew Visual);
//    return 0;
//}