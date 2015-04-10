#include "UI.h"
#include <string.h> 
#include <sstream> 
#include <iostream>
#include <time.h> 

using namespace ChatSocket;


System::String^ getHour(){ //funzione che restituisce l'ora
	time_t rawtime; 
	time(&rawtime); // stringa che contiene giorno, mese, anno, ora , minuti, secondi
	char hour[6]; // dichiaro un vettore di 6 elementi
	int i, j = 11; // dichiaro i e j
	for (i = 0; i < 5;i++,j++) // do i caratteri da 11 a 16 della stringa al vettore hour es: 12:20
		hour[i] = ctime(&rawtime)[j];
	hour[5] = '\0'; // se non metto il \0 inserisce caratteri random dopo l'ora
	System::String^ hour2 = gcnew System::String(hour);
	return hour2;
}

UI::UI() // costruttore
{
	InitializeComponent();

	txtMessage->Enabled = false; // disabilita gli input
	btnSend->Enabled = false; 
	btnDisconnect->Enabled = false;

	client_m = gcnew Mutex(false);

	client = nullptr;
	server = new WinIPv4TcpServer();

	if (!server->bind("0.0.0.0", 12345) || !server->listen()) { // 0.0.0.0 e' l'ep e 12345 è la porta
		OutputDebugString(L"*** [ERROR] UI(): Bind or listen failed!\n");
		
	}
	else {
		OutputDebugString(L"*** UI(): server bind() & listen() successful\n");
		Thread^ t = gcnew Thread(gcnew ThreadStart(this, &UI::acceptConnections)); // inizializza il thread
		t->Start(); // fa partire il thread
	}

	
	
}

UI::~UI() // distruttore
{
	if (components)
		delete components;
}

void UI::InitializeComponent() // inizializza i componenti
{
	this->txtChat = (gcnew System::Windows::Forms::TextBox());
	this->txtMessage = (gcnew System::Windows::Forms::TextBox());
	this->btnSend = (gcnew System::Windows::Forms::Button());
	this->btnDisconnect = (gcnew System::Windows::Forms::Button());
	this->statusStrip1 = (gcnew System::Windows::Forms::StatusStrip());
	this->lblConnectedTo = (gcnew System::Windows::Forms::ToolStripStatusLabel());
	this->textBox1 = (gcnew System::Windows::Forms::TextBox());
	this->label = (gcnew System::Windows::Forms::Label());
	this->statusStrip1->SuspendLayout();
	this->SuspendLayout();
	// 
	// txtChat
	// 
	this->txtChat->BackColor = System::Drawing::SystemColors::GradientActiveCaption;
	this->txtChat->Location = System::Drawing::Point(0, 32);
	this->txtChat->Multiline = true;
	this->txtChat->Name = L"txtChat";
	this->txtChat->ReadOnly = true;
	this->txtChat->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
	this->txtChat->Size = System::Drawing::Size(250, 363);
	this->txtChat->TabIndex = 0;
	// 
	// txtMessage
	// 
	this->txtMessage->Location = System::Drawing::Point(0, 401);
	this->txtMessage->MaxLength = 511;
	this->txtMessage->Name = L"txtMessage";
	this->txtMessage->Size = System::Drawing::Size(381, 20);
	this->txtMessage->TabIndex = 3;
	// 
	// btnSend
	// 
	this->btnSend->Location = System::Drawing::Point(387, 400);
	this->btnSend->Name = L"btnSend";
	this->btnSend->RightToLeft = System::Windows::Forms::RightToLeft::No;
	this->btnSend->Size = System::Drawing::Size(110, 20);
	this->btnSend->TabIndex = 4;
	this->btnSend->Text = L"Invia";
	this->btnSend->UseVisualStyleBackColor = true;
	this->btnSend->Click += gcnew System::EventHandler(this, &UI::btnSend_Click);
	// 
	// btnDisconnect
	// 
	this->btnDisconnect->Location = System::Drawing::Point(387, 6);
	this->btnDisconnect->Name = L"btnDisconnect";
	this->btnDisconnect->Size = System::Drawing::Size(110, 20);
	this->btnDisconnect->TabIndex = 6;
	this->btnDisconnect->Text = L"Disconnetti";
	this->btnDisconnect->UseVisualStyleBackColor = true;
	this->btnDisconnect->Click += gcnew System::EventHandler(this, &UI::btnDisconnect_Click);
	// 
	// statusStrip1
	// 
	this->statusStrip1->BackColor = System::Drawing::SystemColors::Info;
	this->statusStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->lblConnectedTo });
	this->statusStrip1->Location = System::Drawing::Point(0, 428);
	this->statusStrip1->Name = L"statusStrip1";
	this->statusStrip1->Size = System::Drawing::Size(502, 22);
	this->statusStrip1->TabIndex = 7;
	this->statusStrip1->Text = L"statusStrip1";
	// 
	// lblConnectedTo
	// 
	this->lblConnectedTo->Name = L"lblConnectedTo";
	this->lblConnectedTo->Size = System::Drawing::Size(83, 17);
	this->lblConnectedTo->Text = L"Non connesso";
	// 
	// textBox1
	// 
	this->textBox1->BackColor = System::Drawing::SystemColors::GradientActiveCaption;
	this->textBox1->Location = System::Drawing::Point(247, 32);
	this->textBox1->Multiline = true;
	this->textBox1->Name = L"textBox1";
	this->textBox1->ReadOnly = true;
	this->textBox1->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
	this->textBox1->Size = System::Drawing::Size(250, 363);
	this->textBox1->TabIndex = 10;
	// 
	// label
	// 
	this->label->AutoSize = true;
	this->label->Location = System::Drawing::Point(12, 10);
	this->label->Name = L"label";
	this->label->Size = System::Drawing::Size(111, 13);
	this->label->TabIndex = 11;
	this->label->Text = L"Porta utilizzata: 12345";
	// 
	// UI
	// 
	this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
	this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
	this->BackColor = System::Drawing::SystemColors::Info;
	this->ClientSize = System::Drawing::Size(502, 450);
	this->Controls->Add(this->label);
	this->Controls->Add(this->textBox1);
	this->Controls->Add(this->statusStrip1);
	this->Controls->Add(this->btnDisconnect);
	this->Controls->Add(this->btnSend);
	this->Controls->Add(this->txtMessage);
	this->Controls->Add(this->txtChat);
	this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
	this->Name = L"UI";
	this->Text = L"Chat";
	this->statusStrip1->ResumeLayout(false);
	this->statusStrip1->PerformLayout();
	this->ResumeLayout(false);
	this->PerformLayout();

}


System::Void UI::btnSend_Click(System::Object^  sender, System::EventArgs^  e)
{
	using namespace System::Runtime::InteropServices;
	IntPtr ptrToNativeString = Marshal::StringToHGlobalAnsi((String^)"MessageS " + getHour() + " " + txtMessage->Text + "\n");
	//aggiunge Messages prima del messaggio per far capire al client che questo messaggio viene dall'altro utente
	// aggiunge anche l'ora e \n alla fine per andare a capo
	char* msg = static_cast<char*>(ptrToNativeString.ToPointer()); 
	
	if (client == nullptr || !client->send(msg)) {  // invia il messaggio al client
		OutputDebugString(L"*** [ERROR] btnSend_Click(): client->send() failed!\n"); // viene eseguito se non viene mandato il messaggio
		MessageBox::Show("*** [ERROR] btnSend_Click(): client->send() failed!");
	}
	else {//se il messaggio viene inviato al client
		if (txtMessage->Text != String::Empty) { // se il messaggio non è vuoto
			
				txtChat->AppendText("\n"); //  va a capo nella text box di inistra
				textBox1->AppendText(" "+ getHour() + " " + txtMessage->Text + "\n"); // scrive l'ora e il messaggio nella text box di destra
				txtMessage->Clear(); // resetta la text field
			
		}
	}
}


System::Void UI::btnDisconnect_Click(System::Object^  sender, System::EventArgs^  e)
{
	if (client == nullptr)
		return;

	if (MessageBox::Show(gcnew String("Vuoi disconnettere il server?"), "Disconnettere?", MessageBoxButtons::YesNo) == ::DialogResult::Yes) {
		client_m->WaitOne();
		client->disconnect();
		delete client;
		client = nullptr;
		
		client_m->ReleaseMutex();
		txtChat->Clear();
		textBox1->Clear();
		name_sent = false;
	}
}

void UI::updateUIDisconnect()
{
	txtMessage->Enabled = false;
	btnSend->Enabled = false;
	lblConnectedTo->Text = "Non connesso";
	btnDisconnect->Enabled = false;
}

void UI::receiveMessage() // funzione per ricevere i messaggi
{
	OutputDebugString(L"*** receiveMessage(): thread started\n");
	client_m->WaitOne();
	bool flag = false;
	while (client != nullptr && client->connected() ) {
		
			client_m->ReleaseMutex();
			bool err = false;
			std::string msg = client->read(512, err);
			if (flag){ // il primo messaggio che riceve è il nome del client, questo controllo serve per non scriverlo nella text box e non inviarlo
				appendMessage(" " + gcnew String(msg.c_str()));
				client->send("Message " + msg);
			}
			flag = true;
			client_m->WaitOne();
	}
	client_m->ReleaseMutex();

	toggleButtonDisconnetti();
	toggleButtonInvia();
	toggleTextboxMessage();
	setStatusbar(String::Empty);
	name_sent = false;
	emptyChat();


	OutputDebugString(L"*** receiveMessage(): end thread\n");
}

void UI::acceptConnections()
{
	OutputDebugString(L"*** acceptConnections(): thread started\n");

	std::unique_ptr<WinIPv4TcpSocket> c = server->accept();

	OutputDebugString(L"*** acceptConnections(): accept() function returned\n");

	if (c != nullptr) {
		client_m->WaitOne();

		if (!client) {
			client = c.release();
			OutputDebugString(L"*** acceptConnections(): client connected\n");

			Thread^ t = gcnew Thread(gcnew ThreadStart(this, &UI::receiveMessage));
			t->Start(); // fa partire il thread

			toggleButtonDisconnetti();
			toggleButtonInvia();
			toggleTextboxMessage();

			client->send("START_NAME\n"); // invia START_NAME per far capire al client che è connesso
			client->send("NameAccepted\n"); // invia NameAccepted per far capire che ha accettato il nome
			// il client java fuziona anche con un server multi chat, in quel caso non accetterebbe due nomi uguali 

			Net::IPHostEntry^ hostInfo = Net::Dns::GetHostEntry(gcnew String(client->peeraddress().c_str()));
			setStatusbar(hostInfo->HostName);
			//pcName = hostInfo->HostName;
		}

		client_m->ReleaseMutex();

		server->close();
	}
	else {
		OutputDebugString(L"*** acceptConnections(): accept() returned nullptr!");
	}

	OutputDebugString(L"*** acceptConnections(): end thread\n");
}

void UI::appendMessage(String^ msg) // funzione di inserimento nella text box
{
	if (txtChat->InvokeRequired) {
		AppendMessageDelegate^ dlg = gcnew AppendMessageDelegate(this, &UI::appendMessage);
		Invoke(dlg, gcnew array < Object^ > { msg });
	}
	else {
		txtChat->AppendText(msg + "\n"); // inserisce il messaggio nella text box di sinistra
		textBox1->AppendText("\n"); //va a capo nella text box di sinistra
	}
}


void UI::toggleButtonInvia() 
{
	if (btnSend->InvokeRequired) {
		ToggleButtonInviaDelegate^ dlg = gcnew ToggleButtonInviaDelegate(this, &UI::toggleButtonInvia);
		Invoke(dlg);
	}
	else {
		btnSend->Enabled = !btnSend->Enabled;
	}
}

void UI::toggleButtonDisconnetti()
{
	if (btnDisconnect->InvokeRequired) {
		ToggleButtonDisconnettiDelegate^ dlg = gcnew ToggleButtonDisconnettiDelegate(this, &UI::toggleButtonDisconnetti);
		Invoke(dlg);
	}
	else {
		btnDisconnect->Enabled = !btnDisconnect->Enabled;
	}
}

void UI::toggleTextboxMessage()
{
	if (txtMessage->InvokeRequired) {
		ToggleButtonInviaDelegate^ dlg = gcnew ToggleButtonInviaDelegate(this, &UI::toggleTextboxMessage);
		Invoke(dlg);
	}
	else {
		txtMessage->Enabled = !txtMessage->Enabled;
	}
}

void UI::setStatusbar(String^ str)
{
	if (txtMessage->InvokeRequired) {
		SetStatusbarDelegate^ dlg = gcnew SetStatusbarDelegate(this, &UI::setStatusbar);
		Invoke(dlg, gcnew array < String^ > { str});
	}
	else {
		if (str == String::Empty)
			lblConnectedTo->Text = "Non connesso"; // scrive non connesso nella label
		else
			lblConnectedTo->Text = "Connesso al pc " + str; // scrive connesso + nome pc nella label
	}
}

void UI::emptyChat() // pulisce text box
{
	if (txtChat->InvokeRequired) {
		EmptyChatDelegate^ dlg = gcnew EmptyChatDelegate(this, &UI::emptyChat);
		Invoke(dlg);
	}
	else {
		txtChat->Clear();
		textBox1->Clear();
	}
}


[STAThread]
void Main(array<String^>^ args)
{
	if (!WinSockLibManager::init())
		return;

	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	ChatSocket:UI ui;
	Application::Run(%ui);

	WinSockLibManager::clean();
}