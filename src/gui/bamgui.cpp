#include "bamgui.h"
#include "chatwindow.h"

namespace Gui{
	void BamGui::buttonClick(){

	}

	void BamGui::init(){
		// Create our main SFGUI window
		auto window = sfg::Window::Create();
		window->SetTitle( "Title" );

		mDesktop.Add(window);

		// Create widgets
		auto button = sfg::Button::Create();
		button->SetLabel( "Set" );
		button->GetSignal( sfg::Widget::OnLeftClick ).Connect( std::bind( &BamGui::buttonClick, this ) );

		mLabel = sfg::Label::Create();
		mLabel->SetText( "no text yet" );

		mEntry = sfg::Entry::Create();
		mEntry->SetRequisition( sf::Vector2f( 80.f, 0.f ) );

		// Create an pack into box
		auto box1 = sfg::Box::Create( sfg::Box::Orientation::VERTICAL );
		auto box2 = sfg::Box::Create( sfg::Box::Orientation::HORIZONTAL );

		box1->Pack( mLabel );
		box2->Pack( mEntry );
		box2->Pack( button );
		box1->Pack( box2 );

		box1->SetSpacing( 5.f );
		window->Add( box1 );
	}

	void BamGui::handleEvent(sf::Event& event){
		mDesktop.HandleEvent(event);
	}

	void BamGui::display(sf::RenderWindow& target){
		target.pushGLStates();
		target.resetGLStates();
		mSfgui.Display(target);
		target.popGLStates();
	}

	void BamGui::logic(){
		mDesktop.Update( 0.04f );
	}

	bool BamGui::hasFocus(){
		return mEntry->HasFocus();
	}
}
