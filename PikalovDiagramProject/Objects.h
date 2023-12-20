#pragma once

using namespace System;
using namespace System::Windows::Forms;
using namespace System::Drawing;
using namespace System::Collections::Generic;

namespace PikalovDiagramProject {

	public ref class GraphicElement {
	public:
		Point^ point = Point(0, 0);

		Void virtual drawing(Graphics^ g) = 0;
	};

	public ref class Element : GraphicElement {
	public:
		String^ title = "Title";
		int height = 150;
		int width = 220;
		bool isInside(Point^ cursorPoint) {
			if (cursorPoint->X >= point->X && cursorPoint->X <= point->X + width &&
				cursorPoint->Y >= point->Y && cursorPoint->Y <= point->Y + height) {
				return true;
			}
			return false;
		}
	};

	public ref class ClassObject : Element {
	public:
		String^ properties = "+button1: Button\n+button1: Button";
		String^ methods = "+MainForm(): Void\n+MainForm(): Void";

		Void drawing(Graphics^ g) override {
			// Создаем кисть для отрисовки
			SolidBrush^ brush = gcnew SolidBrush(Color::White);
			Pen^ pen = gcnew Pen(Color::Black, 1);

			// Отрисовываем прямоугольник с учетом размеров и положения
			g->FillRectangle(brush, point->X, point->Y, width, height);
			g->DrawRectangle(pen, point->X, point->Y, width, height);

			// Рисуем текст заголовка
			g->DrawString(title, gcnew System::Drawing::Font("Arial", 10), Brushes::Black, point->X + (width - g->MeasureString(title, gcnew System::Drawing::Font("Arial", 10)).Width) / 2, point->Y);

			g->DrawString(properties, gcnew System::Drawing::Font("Arial", 10), Brushes::Black, point->X, point->Y + g->MeasureString(title, gcnew System::Drawing::Font("Arial", 10)).Height);
			g->DrawLine(pen, Point(point->X, point->Y + g->MeasureString(title, gcnew System::Drawing::Font("Arial", 10)).Height), Point(point->X + width, point->Y + g->MeasureString(title, gcnew System::Drawing::Font("Arial", 10)).Height));
			g->DrawString(methods, gcnew System::Drawing::Font("Arial", 10), Brushes::Black, point->X, point->Y + g->MeasureString(properties, gcnew System::Drawing::Font("Arial", 10)).Height + g->MeasureString(title, gcnew System::Drawing::Font("Arial", 10)).Height);
			g->DrawLine(pen, Point(point->X, point->Y + g->MeasureString(properties, gcnew System::Drawing::Font("Arial", 10)).Height + g->MeasureString(title, gcnew System::Drawing::Font("Arial", 10)).Height), Point(point->X + width, point->Y + g->MeasureString(properties, gcnew System::Drawing::Font("Arial", 10)).Height + g->MeasureString(title, gcnew System::Drawing::Font("Arial", 10)).Height));
		}
		
	};

	public ref class InterfaceObject : Element {
	public:

		Void drawing(Graphics^ g) override {
			// Создаем кисть для отрисовки
			SolidBrush^ brush = gcnew SolidBrush(Color::White);
			Pen^ pen = gcnew Pen(Color::Black, 1);

			// Отрисовываем прямоугольник с учетом размеров и положения
			g->FillRectangle(brush, point->X, point->Y, width, height);
			g->DrawRectangle(pen, point->X, point->Y, width, height);

			// Рисуем текст заголовка
			g->DrawString("«interface»", gcnew System::Drawing::Font("Arial", 10), Brushes::Black, point->X + (width - g->MeasureString("«interface»", gcnew System::Drawing::Font("Arial", 10)).Width) / 2, point->Y);

			g->DrawString(title, gcnew System::Drawing::Font("Arial", 10), Brushes::Black, point->X + (width - g->MeasureString(title, gcnew System::Drawing::Font("Arial", 10)).Width) / 2, point->Y + g->MeasureString(title, gcnew System::Drawing::Font("Arial", 10)).Height);
			}

	};

	public ref class ObjectObject : Element {
	public:

		Void drawing(Graphics^ g) override {
			// Создаем кисть для отрисовки
			SolidBrush^ brush = gcnew SolidBrush(Color::White);
			Pen^ pen = gcnew Pen(Color::Black, 1);

			// Отрисовываем прямоугольник с учетом размеров и положения
			g->FillRectangle(brush, point->X, point->Y, width, height);
			g->DrawRectangle(pen, point->X, point->Y, width, height);

			// Рисуем текст заголовка
			g->DrawString(title, gcnew System::Drawing::Font("Arial", 10), Brushes::Black, point->X + (width - g->MeasureString(title, gcnew System::Drawing::Font("Arial", 10)).Width) / 2, point->Y + g->MeasureString(title, gcnew System::Drawing::Font("Arial", 10)).Height);
		}

	};

	public ref class ConnectionObject : GraphicElement {
	public:
		bool IsPointOnLine(Point p, Point start, Point end) {
			double d1 = Distance(start, p);
			double d2 = Distance(p, end);
			double lineLength = Distance(start, end);

			// Погрешность, которую вы можете установить в зависимости от ваших потребностей
			double epsilon = 0.5;

			// Проверяем, находится ли точка на линии
			return Math::Abs(d1 + d2 - lineLength) < epsilon;
		}

		double Distance(Point p1, Point p2) {
			// Вычисляем расстояние между двумя точками
			int dx = p2.X - p1.X;
			int dy = p2.Y - p1.Y;
			return Math::Sqrt(dx * dx + dy * dy);
		}

		bool IsClickOnConnection(Point clickPoint) {
			// Проверяем, был ли клик на стрелке
			for (int i = 0; i < Points->Count - 1; i++) {
				if (IsPointOnLine(clickPoint, Points[i], Points[i + 1])) {
					return true;
				}
			}

			return false;
		}
		List<Point>^ Points = gcnew List<Point>();
	};

	public ref class AssociatedObject : ConnectionObject {
	public:
		void DrawArrow(Graphics^ g, Point start, Point end, Pen^ pen) {
			// Рисование основной линии
			g->DrawLine(pen, start, end);

			// Вычисление направления стрелки
			int arrowSize = 20; // Размер стрелки (можете настроить под свои нужды)
			double angle = Math::Atan2(end.Y - start.Y, end.X - start.X);

			// Вычисление координат концов стрелки
			int arrowEndX1 = end.X - static_cast<int>(arrowSize * Math::Cos(angle - Math::PI / 6));
			int arrowEndY1 = end.Y - static_cast<int>(arrowSize * Math::Sin(angle - Math::PI / 6));
			int arrowEndX2 = end.X - static_cast<int>(arrowSize * Math::Cos(angle + Math::PI / 6));
			int arrowEndY2 = end.Y - static_cast<int>(arrowSize * Math::Sin(angle + Math::PI / 6));


			g->DrawLine(pen, end, Point(arrowEndX1, arrowEndY1));
			g->DrawLine(pen, end, Point(arrowEndX2, arrowEndY2));
		}
		Void drawing(Graphics^ g) override {
			Pen^ pen = gcnew Pen(Color::Black, 2);
			Point lastPoint = Point(0, 0);

			if (Points->Count < 2) {
				return;
			}

			for (int i = 0; i < Points->Count; i++) {
				if (i + 1 == Points->Count) {
					break;
				}
				g->DrawLine(pen, Points[i], Points[i + 1]);
			}
			DrawArrow(g, Points[Points->Count - 2], Points[Points->Count - 1], pen);
		}
	};

	public ref class AggregatedObject : ConnectionObject {
	public:
		void DrawArrow(Graphics^ g, Point start, Point end, Pen^ pen) {
			// Рисование основной линии
			g->DrawLine(pen, start, end);

			// Вычисление направления стрелки
			int arrowSize = 20; // Размер стрелки (можете настроить под свои нужды)
			double angle = Math::Atan2(end.Y - start.Y, end.X - start.X);

			// Вычисление координат концов стрелки
			int arrowEndX1 = end.X - static_cast<int>(arrowSize * Math::Cos(angle - Math::PI / 6));
			int arrowEndY1 = end.Y - static_cast<int>(arrowSize * Math::Sin(angle - Math::PI / 6));
			int arrowEndX2 = end.X - static_cast<int>(arrowSize * Math::Cos(angle + Math::PI / 6));
			int arrowEndY2 = end.Y - static_cast<int>(arrowSize * Math::Sin(angle + Math::PI / 6));

			//g->DrawLine(pen, end, Point(arrowEndX1, arrowEndY1));
			//g->DrawLine(pen, end, Point(arrowEndX2, arrowEndY2));

			int arrowEndX3 = end.X - static_cast<int>(arrowSize * 2 * Math::Cos(angle));
			int arrowEndY3 = end.Y - static_cast<int>(arrowSize * 2*  Math::Sin(angle));

			//g->DrawLine(pen, Point(arrowEndX3, arrowEndY3), Point(arrowEndX1, arrowEndY1));
			//g->DrawLine(pen, Point(arrowEndX3, arrowEndY3), Point(arrowEndX2, arrowEndY2));

			SolidBrush^ brush = gcnew SolidBrush(Color::White);
			// Отрисовываем прямоугольник с учетом размеров и положения
			

			array<Point>^ arrowPoints = { end, Point(arrowEndX1, arrowEndY1), Point(arrowEndX3, arrowEndY3), Point(arrowEndX2, arrowEndY2) };
			g->FillPolygon(brush, arrowPoints);
			g->DrawPolygon(pen, arrowPoints);
		}
		Void drawing(Graphics^ g) override {
			Pen^ pen = gcnew Pen(Color::Black, 2);
			Point lastPoint = Point(0, 0);

			if (Points->Count < 2) {
				return;
			}

			for (int i = 0; i < Points->Count; i++) {
				if (i + 1 == Points->Count) {
					break;
				}
				g->DrawLine(pen, Points[i], Points[i + 1]);
			}
			DrawArrow(g, Points[Points->Count - 2], Points[Points->Count - 1], pen);
		}
	};
	
	public ref class ComposedObject : ConnectionObject {
	public:
		void DrawArrow(Graphics^ g, Point start, Point end, Pen^ pen) {
			// Рисование основной линии
			g->DrawLine(pen, start, end);

			// Вычисление направления стрелки
			int arrowSize = 20; // Размер стрелки (можете настроить под свои нужды)
			double angle = Math::Atan2(end.Y - start.Y, end.X - start.X);

			// Вычисление координат концов стрелки
			int arrowEndX1 = end.X - static_cast<int>(arrowSize * Math::Cos(angle - Math::PI / 6));
			int arrowEndY1 = end.Y - static_cast<int>(arrowSize * Math::Sin(angle - Math::PI / 6));
			int arrowEndX2 = end.X - static_cast<int>(arrowSize * Math::Cos(angle + Math::PI / 6));
			int arrowEndY2 = end.Y - static_cast<int>(arrowSize * Math::Sin(angle + Math::PI / 6));

			//g->DrawLine(pen, end, Point(arrowEndX1, arrowEndY1));
			//g->DrawLine(pen, end, Point(arrowEndX2, arrowEndY2));

			int arrowEndX3 = end.X - static_cast<int>(arrowSize * 2 * Math::Cos(angle));
			int arrowEndY3 = end.Y - static_cast<int>(arrowSize * 2 * Math::Sin(angle));

			//g->DrawLine(pen, Point(arrowEndX3, arrowEndY3), Point(arrowEndX1, arrowEndY1));
			//g->DrawLine(pen, Point(arrowEndX3, arrowEndY3), Point(arrowEndX2, arrowEndY2));

			SolidBrush^ brush = gcnew SolidBrush(Color::Black);
			// Отрисовываем прямоугольник с учетом размеров и положения


			array<Point>^ arrowPoints = { end, Point(arrowEndX1, arrowEndY1), Point(arrowEndX3, arrowEndY3), Point(arrowEndX2, arrowEndY2) };
			g->FillPolygon(brush, arrowPoints);
			g->DrawPolygon(pen, arrowPoints);
		}
		Void drawing(Graphics^ g) override {
			Pen^ pen = gcnew Pen(Color::Black, 2);
			Point lastPoint = Point(0, 0);

			if (Points->Count < 2) {
				return;
			}

			for (int i = 0; i < Points->Count; i++) {
				if (i + 1 == Points->Count) {
					break;
				}
				g->DrawLine(pen, Points[i], Points[i + 1]);
			}
			DrawArrow(g, Points[Points->Count - 2], Points[Points->Count - 1], pen);
		}
	};

	public ref class DependentObject : ConnectionObject {
	public:
		void DrawArrow(Graphics^ g, Point start, Point end, Pen^ pen) {
			// Рисование основной линии
			g->DrawLine(pen, start, end);

			// Вычисление направления стрелки
			int arrowSize = 20; // Размер стрелки (можете настроить под свои нужды)
			double angle = Math::Atan2(end.Y - start.Y, end.X - start.X);

			// Вычисление координат концов стрелки
			int arrowEndX1 = end.X - static_cast<int>(arrowSize * Math::Cos(angle - Math::PI / 6));
			int arrowEndY1 = end.Y - static_cast<int>(arrowSize * Math::Sin(angle - Math::PI / 6));
			int arrowEndX2 = end.X - static_cast<int>(arrowSize * Math::Cos(angle + Math::PI / 6));
			int arrowEndY2 = end.Y - static_cast<int>(arrowSize * Math::Sin(angle + Math::PI / 6));

			Pen^ pen2 = gcnew Pen(Color::Black, 2);
			g->DrawLine(pen2, end, Point(arrowEndX1, arrowEndY1));
			g->DrawLine(pen2, end, Point(arrowEndX2, arrowEndY2));
		}
		Void drawing(Graphics^ g) override {
			Pen^ pen = gcnew Pen(Color::Black, 2);
			pen->DashStyle = Drawing2D::DashStyle::Dash;
			Point lastPoint = Point(0, 0);

			if (Points->Count < 2) {
				return;
			}

			for (int i = 0; i < Points->Count; i++) {
				if (i + 1 == Points->Count) {
					break;
				}
				g->DrawLine(pen, Points[i], Points[i + 1]);
			}
			DrawArrow(g, Points[Points->Count - 2], Points[Points->Count - 1], pen);
		}
	};

	public ref class InheritanceObject : ConnectionObject {
	public:
		void DrawArrow(Graphics^ g, Point start, Point end, Pen^ pen) {
			// Рисование основной линии
			g->DrawLine(pen, start, end);

			// Вычисление направления стрелки
			int arrowSize = 20; // Размер стрелки (можете настроить под свои нужды)
			double angle = Math::Atan2(end.Y - start.Y, end.X - start.X);

			// Вычисление координат концов стрелки
			int arrowEndX1 = end.X - static_cast<int>(arrowSize * Math::Cos(angle - Math::PI / 6));
			int arrowEndY1 = end.Y - static_cast<int>(arrowSize * Math::Sin(angle - Math::PI / 6));
			int arrowEndX2 = end.X - static_cast<int>(arrowSize * Math::Cos(angle + Math::PI / 6));
			int arrowEndY2 = end.Y - static_cast<int>(arrowSize * Math::Sin(angle + Math::PI / 6));

			SolidBrush^ brush = gcnew SolidBrush(Color::White);

			array<Point>^ arrowPoints = { end, Point(arrowEndX1, arrowEndY1), Point(arrowEndX2, arrowEndY2) };
			g->FillPolygon(brush, arrowPoints);
			g->DrawPolygon(pen, arrowPoints);
		}
		Void drawing(Graphics^ g) override {
			Pen^ pen = gcnew Pen(Color::Black, 2);
			Point lastPoint = Point(0, 0);
			if (Points->Count < 2) {
				return;
			}

			for (int i = 0; i < Points->Count; i++) {
				if (i + 1 == Points->Count) {
					break;
				}
				g->DrawLine(pen, Points[i], Points[i + 1]);
			}
			DrawArrow(g, Points[Points->Count - 2], Points[Points->Count - 1], pen);
		}
	};

};
