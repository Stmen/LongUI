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


// LongUI namespace
namespace LongUI{
    // default slider control Ĭ�ϻ���ؼ�
    class LongUIAPI UISlider : public UIControl {
        enum UnusedIndex{
            Unused_MouseClickIn = 0,
            UNUSED_SIZE
        };
    private:
        // ��������
        using Super = UIControl ;
    public:
        // Render ��Ⱦ --- ���ڵ�һλ!
        virtual auto LongUIMethodCall Render() noexcept->HRESULT override;
        // do event �¼�����
        virtual bool LongUIMethodCall DoEvent(LongUI::EventArgument&) noexcept override;
        // Ԥ��Ⱦ
        virtual void LongUIMethodCall PreRender() noexcept override {};
        // recreate �ؽ�
        //virtual auto LongUIMethodCall Recreate(LongUIRenderTarget*) noexcept->HRESULT override;
        // close this control �رտؼ�
        virtual void LongUIMethodCall Close() noexcept override;
    public:
        // create ����
        static UIControl* WINAPI CreateControl(pugi::xml_node) noexcept;
        // register Value Changed ע��䶯�¼�
        LongUIInline void RegisterValueChangedEvent(LongUICallBack call, UIControl* target) noexcept {
            m_eventChanged = call; m_pChangedTarget = target;
        };
        // ��ȡ��ֵ
        LongUIInline auto GetValue() { return m_fValue; }
    protected:
        // constructor ���캯��
        UISlider(pugi::xml_node) noexcept;
        // destructor ��������
        ~UISlider() noexcept { }
        // deleted function
        UISlider(const UISlider&) = delete;
    protected:
        // changed event target
        UIControl*          m_pChangedTarget = nullptr;
        // changed event
        LongUICallBack      m_eventChanged = nullptr;
        // slider rect
        D2D1_RECT_F         m_rcSlider = D2D1::RectF();
        // value range[0, 1]
        float               m_fValue = 0.f;
        // value range[0, 1]
        float               m_fValueOld = 0.f;
        // start
        float               m_fStart = 0.f;
        // end
        float               m_fEnd = 100.f;
        // slider half width
        float               m_fSliderHalfWidth = static_cast<float>(LongUIDefaultSliderHalfWidth);
    };
}