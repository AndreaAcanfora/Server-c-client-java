#pragma once

#include <vector>
#include "WinNetTools.h"
#include "winsockwrap.h"



namespace ChatSocket {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Threading;

	public ref class UI : public System::Windows::Forms::Form
	{
	public:
		UI();

		delegate void AppendHostnameDelegate(String^ text);
		

		delegate void AppendMessageDelegate(String^ msg);
		void appendMessage(String^ msg);

		delegate void ToggleButtonInviaDelegate();
		void toggleButtonInvia();

		delegate void ToggleButtonDisconnettiDelegate();
		void toggleButtonDisconnetti();

		delegate void ToggleTextboxMessageDelegate();
		void toggleTextboxMessage();

		delegate void SetStatusbarDelegate(String^ msg);
		void setStatusbar(String^ msg);

		delegate void EmptyChatDelegate();
		void emptyChat();

	protected:
		~UI();

	private:
		System::ComponentModel::Container ^components;
		System::Windows::Forms::TextBox^  txtChat;


		System::Windows::Forms::TextBox^  txtMessage;
		System::Windows::Forms::Button^  btnSend;
		System::Windows::Forms::Button^  btnDisconnect;

		System::Windows::Forms::StatusStrip^  statusStrip1;


	private: System::Windows::Forms::TextBox^  textBox1;
	private: System::Windows::Forms::Label^  label;

			 System::Windows::Forms::ToolStripStatusLabel^  lblConnectedTo;

		void InitializeComponent();

		
		System::Void btnSend_Click(System::Object^  sender, System::EventArgs^  e);
		System::Void btnDisconnect_Click(System::Object^  sender, System::EventArgs^  e);

		void acceptConnections();
		void receiveMessage();

		void updateUIDisconnect();

		WinIPv4TcpSocket* client = nullptr;
		WinIPv4TcpServer* server = nullptr;

		Mutex^ client_m;

		String^ nameClient;	

		bool name_sent = false;

	

	};
}
