#pragma once

#include "Objects.h"

using namespace System;
using namespace System::Windows::Forms;
using namespace System::Drawing;
using namespace System::Collections::Generic;


namespace PikalovDiagramProject {
	public ref class DrawingManager {
	private:
		List<Element^>^ Elements = gcnew List<Element^>();
		List<ConnectionObject^>^ Connections = gcnew List<ConnectionObject^>();
	public:
		
		ConnectionObject^ AddAssociatedObject() {
			ConnectionObject^ connectionObject = gcnew AssociatedObject();
			Connections->Add(connectionObject);
			return connectionObject;
		}

		ConnectionObject^ AddAggregatedObject() {
			ConnectionObject^ connectionObject = gcnew AggregatedObject();
			Connections->Add(connectionObject);
			return connectionObject;
		}

		ConnectionObject^ AddComposedObject() {
			ConnectionObject^ connectionObject = gcnew ComposedObject();
			Connections->Add(connectionObject);
			return connectionObject;
		}

		ConnectionObject^ AddDependentObject() {
			ConnectionObject^ connectionObject = gcnew DependentObject();
			Connections->Add(connectionObject);
			return connectionObject;
		}

		ConnectionObject^ AddInheritanceObject() {
			ConnectionObject^ connectionObject = gcnew InheritanceObject();
			Connections->Add(connectionObject);
			return connectionObject;
		}

		Void AddClassObject() {
			Elements->Add(gcnew ClassObject());
		}

		Void RemoveObject(Element^ remove_element) {
			for (int i = 0; i < Elements->Count; i++) {
				if (remove_element == Elements[i]) {
					Elements->RemoveAt(i);
					return;
				}
			}
		}

		Void RemoveConnection(ConnectionObject^ remove_element) {
			for (int i = 0; i < Connections->Count; i++) {
				if (remove_element == Connections[i]) {
					Connections->RemoveAt(i);
					return;
				}
			}
		}

		Void AddInterfaceObject() {
			Elements->Add(gcnew InterfaceObject());
		}

		Void AddObjectObject() {
			Elements->Add(gcnew ObjectObject());
		}

		Element^ GetElement(Point cursorPoint) {
			for each (Element ^ element in Elements)
			{
				if (element->isInside(cursorPoint)) {
					return element;
				}
			}
			return nullptr;
		}

		ConnectionObject^ GetConnection(Point cursorPoint) {
			for each (ConnectionObject^ element in Connections)
			{
				if (element->IsClickOnConnection(cursorPoint)) {
					return element;
				}
			}
			return nullptr;
		}

		Void DrawPictureBoxImage(PictureBox^ pictureBox1) {
			Bitmap^ notebookBitmap = gcnew Bitmap(pictureBox1->Width, pictureBox1->Height);

			// Получаем Graphics для Bitmap
			Graphics^ g = Graphics::FromImage(notebookBitmap);
			Pen^ pen = gcnew Pen(Color::Black);

			// Задаем шаг сетки (расстояние между линиями)
			int step = 20;

			// Рисуем вертикальные линии
			for (int x = 0; x < g->VisibleClipBounds.Width; x += step) {
				g->DrawLine(pen, (float)x, (float)0, (float)x, (float)g->VisibleClipBounds.Height);
			}

			// Рисуем горизонтальные линии
			for (int y = 0; y < g->VisibleClipBounds.Height; y += step) {
				g->DrawLine(pen, (float)0, (float)y, (float)g->VisibleClipBounds.Width, (float)y);
			}

			for each (Element^ element in Elements)
			{
				element->drawing(g);
			}

			for each (ConnectionObject^ element in Connections)
			{
				element->drawing(g);
			}

			// Освобождаем ресурсы объекта Pen
			delete pen;
			pictureBox1->Image = notebookBitmap;
		}
	};
};
