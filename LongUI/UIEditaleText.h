#pragma once
/**
* Copyright (c) 2014-2015 dustpg   mailto:dustpg@gmail.com
*
* Permission is hereby granted, free of charge, to any person
* obtaining a copy of this software and associated documentation
* files (the "Software"), to deal in the Software without
* restriction, including without limitation the rights to use,
* copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following
* conditions:
*
* The above copyright notice and this permission notice shall be
* included in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
* OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
* HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
* OTHER DEALINGS IN THE SOFTWARE.
*/


// longui namespace
namespace LongUI {
    // the operation marco of TextType
#define UIEditaleText_IsRiched ((this->type & Type_Riched) > 0)
#define UIEditaleText_IsMultiLine ((this->type & Type_MultiLine) > 0)
#define UIEditaleText_IsReadOnly ((this->type & Type_ReadOnly) > 0)
#define EditaleTextType_SetTrue(value, type) value |= type 
#define EditaleTextType_SetFalse(value, type) value &= ~type 
    // core editable text component
    class LongUIAlignas CUIEditaleText {
        // ��д
        using HitTestMetrics = DWRITE_HIT_TEST_METRICS;
        // ������
        using MetricsBuffer = SimpleSmallBuffer<HitTestMetrics, 8>;
    public:
        // the mode of text selection zone ѡ����ģʽ
        enum SelectionMode : uint32_t {
            Mode_Left = 0,      // ����
            Mode_Right,         // ����
            Mode_Up,            // ������
            Mode_Down,          // ���½�
            Mode_LeftChar,      // ���ƶ�һ���ַ�
            Mode_RightChar,     // ���ƶ�һ���ַ�
            Mode_LeftWord,      // ���ƶ�һ������
            Mode_RightWord,     // ���ƶ�һ������
            Mode_Home,          // ����ʼλ��
            Mode_End,           // �ܽ���Ϊֹ
            Mode_First,         // ����ʼλ��
            Mode_Last,          // �н���Ϊֹ
            Mode_Leading,       // �����λ��
            Mode_Trailing,      // ѡ���Ե
            Mode_SelectAll,     // ȫѡ
        };
        // the type of editable text, bit array
        // same as TXTBIT_XXXXXXX in TextServ.h
        enum EditaleTextType : uint32_t {
            Type_All = uint32_t(-1),
            Type_None = 0,
            // rich��ED��(not richer more than RICHTEXT(UIRichEdit))
            Type_Riched = 1 << 0,
            // multi-line
            Type_MultiLine = 1 << 1,
            // read-only
            Type_ReadOnly = 1 << 2,
            // underline accelerator character
            Type_Accelerator = 1 << 3,
            // use password
            Type_Password = 1 << 4,
        };
        // copy the global properties for layout
        static void __fastcall CopyGlobalProperties(IDWriteTextLayout*, IDWriteTextLayout*) noexcept;
        // copy the single prop for range
        static void __fastcall CopySinglePropertyRange(IDWriteTextLayout*, uint32_t, IDWriteTextLayout*, uint32_t, uint32_t) noexcept;
        // copy the range prop for layout
        static void __fastcall CopyRangedProperties(IDWriteTextLayout*, IDWriteTextLayout*, uint32_t, uint32_t, uint32_t, bool = false) noexcept;
    public: // �ⲿ������
        // set new size
        LongUIInline auto SetNewSize(float w, float h) noexcept {
            m_size.width = w; m_size.height = h;
            this->layout->SetMaxWidth(w); this->layout->SetMaxHeight(h);
        }
        // get hittest
        LongUIInline auto GetHitTestMetrics() noexcept { return m_metriceBuffer.data; }
        // get hittest's length 
        LongUIInline auto GetHitTestLength() noexcept { return m_metriceBuffer.data_length; }
        // c-style string
        LongUIInline auto c_str() const noexcept { return m_text.c_str(); }
        // c-style string
        LongUIInline auto assign(const wchar_t* str, size_t length) noexcept { m_text.assign(str, length); return recreate_layout(); }
    private:
        // refresh, while layout chenged, should be refreshed
        auto __fastcall refresh(bool =true) noexcept ->UIWindow*;
        // recreate layout
        void __fastcall recreate_layout() noexcept;
        // insert text
        auto __fastcall insert(const wchar_t* str, uint32_t pos, uint32_t length) noexcept->HRESULT;
    public: // һ���ڲ�������
        // get selection range
        auto __fastcall GetSelectionRange()const noexcept->DWRITE_TEXT_RANGE;
        // set selection zone
        auto __fastcall SetSelection(SelectionMode, uint32_t, bool, bool = true) noexcept->HRESULT;
        // delete the selection text
        auto __fastcall DeleteSelection() noexcept->HRESULT;
        // set selection
        bool __fastcall SetSelectionFromPoint(float x, float y, bool extendSelection) noexcept;
    public: // Event
        // when drop
        bool __fastcall OnDrop(IDataObject* data, DWORD* effect) noexcept { this->PasteFromGlobal(m_recentMedium.hGlobal); return true; }
        // when drag enter
        bool __fastcall OnDragLeave() noexcept { m_bDragFromThis = false; return true; }
        // when drag enter
        bool __fastcall OnDragEnter(IDataObject* data, DWORD* effect) noexcept;
        // when drag enter: relative postion
        bool __fastcall OnDragOver(float x, float y) noexcept;
        // recreate
        void __fastcall Recreate(ID2D1RenderTarget*) noexcept;
        // when key character
        void __fastcall OnChar(char32_t) noexcept;
        // when key pressed
        void __fastcall OnKey(uint32_t) noexcept;
        // when get focus
        void __fastcall OnSetFocus() noexcept;
        // when kill focus
        void __fastcall OnKillFocus() noexcept;
        // on L button up: relative postion
        void __fastcall OnLButtonUp(float x, float y) noexcept;
        // on L button down: relative postion
        void __fastcall OnLButtonDown(float x, float y, bool =false) noexcept;
        // on L button hold && move: relative postion
        void __fastcall OnLButtonHold(float x, float y, bool = false) noexcept;
    public: // һ���ⲿ������
        // align caret
        void __fastcall AlignCaretToNearestCluster(bool, bool = false) noexcept;
        // get the caret rect
        void __fastcall GetCaretRect(RectLTWH_F&) noexcept;
        // render this: absolute postion
        void __fastcall Render(float x, float y) noexcept;
        // refresh the selection HitTestMetrics
        void __fastcall RefreshSelectionMetrics(DWRITE_TEXT_RANGE) noexcept;
        // copy to HGLOBAL
        auto __fastcall CopyToGlobal() noexcept->HGLOBAL;
        // copy to clipboard
        auto __fastcall CopyToClipboard() noexcept->HRESULT;
        // paste form HGlobal
        auto __fastcall PasteFromGlobal(HGLOBAL) noexcept->HRESULT;
        // paste form HGlobal
        auto __fastcall PasteFromClipboard() noexcept->HRESULT;
    public:
        // get line no.
        static void GetLineFromPosition(
            const DWRITE_LINE_METRICS* lineMetrics,
            uint32_t lineCount,
            uint32_t textPosition,
            OUT uint32_t* lineOut,
            OUT uint32_t* linePositionOut
            ) noexcept;
    public:
        // destructor
        ~CUIEditaleText() noexcept;
        // constructor
        CUIEditaleText(UIControl*, pugi::xml_node, const char* prefix = "text") noexcept;
        // type of text
        IDWriteTextLayout*      layout = nullptr;
        // type of text
        EditaleTextType         type = Type_None;
        // password char
        char32_t                password = U'*';
    private:
        // render target
        ID2D1RenderTarget*      m_pRenderTarget = nullptr;
        // selection brush
        ID2D1SolidColorBrush*   m_pSelectionColor = nullptr;
        // host control
        UIControl*              m_pHost = nullptr;
        // drop source
        CUIDropSource*          m_pDropSource = CUIDropSource::New();
        // drop source
        CUIDataObject*          m_pDataObject = CUIDataObject::New();
        // click start point
        D2D1_POINT_2F           m_ptStart = D2D1::Point2F();
        // size of this
        D2D1_SIZE_F             m_size = D2D1::SizeF(96.f, 96.f);
        // Text Renderer
        UIBasicTextRenderer*    m_pTextRenderer = nullptr;
        // factory
        IDWriteFactory*         m_pFactory = nullptr;
        // basic text format
        IDWriteTextFormat*      m_pBasicFormat = nullptr;
        // drag text range
        DWRITE_TEXT_RANGE       m_dragRange;
        // click in selection
        bool                    m_bClickInSelection = false;
        // drag start
        bool                    m_bThisFocused = false;
        // drag format supported
        bool                    m_bDragFormatOK = false;
        // drag data from this
        bool                    m_bDragFromThis = false;
        // stg medium
        STGMEDIUM               m_recentMedium;
        // hit test metrics
        MetricsBuffer           m_metriceBuffer;
        // the anchor of caret ���êλ -- ��ǰλ��
        uint32_t                m_u32CaretAnchor = 0;
        // the position of caret ���λ�� -- ��ʼλ��
        uint32_t                m_u32CaretPos = 0;
        // the pos offset of caret ���ƫ�� -- ѡ������С
        uint32_t                m_u32CaretPosOffset = 0;
        // string of text
        DynamicString           m_text;
        // basic color
        D2D1_COLOR_F            m_basicColor;
        // context buffer for text renderer
        ContextBuffer           m_buffer;
    };
    // Needed text editor backspace deletion.
    static inline bool IsSurrogate(uint32_t ch) noexcept {
        // 0xD800 <= ch <= 0xDFFF
        return (ch & 0xF800) == 0xD800;
    }
    static inline bool IsHighSurrogate(uint32_t ch) noexcept {
        // 0xD800 <= ch <= 0xDBFF
        return (ch & 0xFC00) == 0xD800;
    }
    static inline bool IsLowSurrogate(uint32_t ch) noexcept {
        // 0xDC00 <= ch <= 0xDFFF
        return (ch & 0xFC00) == 0xDC00;
    }
}