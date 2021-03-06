#ifndef __ELASTOS_DROID_VIEW_CONTEXTTHEMEWRAPPER_H__
#define __ELASTOS_DROID_VIEW_CONTEXTTHEMEWRAPPER_H__

#include "content/ContextWrapper.h"

using Elastos::Droid::Content::IContext;

namespace Elastos {
namespace Droid {
namespace View {

class ContextThemeWrapper : public Elastos::Droid::Content::ContextWrapper
{
public:
    ContextThemeWrapper();

    ContextThemeWrapper(
        /* [in] */ IContext* base,
        /* [in] */ Int32 themeres);

    /**
     * Call to set an "override configuration" on this context -- this is
     * a configuration that replies one or more values of the standard
     * configuration that is applied to the context.  See
     * {@link Context#createConfigurationContext(Configuration)} for more
     * information.
     *
     * <p>This method can only be called once, and must be called before any
     * calls to {@link #getResources()} are made.
     */
    CARAPI ApplyOverrideConfiguration(
        /* [in] */ IConfiguration* overrideConfiguration);

    CARAPI GetResources(
        /* [out] */ IResources** resources);

    CARAPI SetTheme(
        /* [in] */ Int32 resid);

    CARAPI GetThemeResId(
        /* [out] */ Int32* resId);

    CARAPI GetTheme(
        /* [out] */ IResourcesTheme** theme);

    CARAPI GetSystemService(
        /* [in] */ const String& name,
        /* [out] */ IInterface** object);

protected:
    CARAPI AttachBaseContext(
        /* [in] */ IContext* base);

    CARAPI OnApplyThemeResource(
        /* [in] */ IResourcesTheme* theme,
        /* [in] */ Int32 resid,
        /* [in] */ Boolean first);

    CARAPI Init(
        /* [in] */ IContext* base,
        /* [in] */ Int32 themeres);

private:
    CARAPI InitializeTheme();

private:
    Int32 mThemeResource;
    AutoPtr<IResourcesTheme> mTheme;
    AutoPtr<ILayoutInflater> mInflater;
    AutoPtr<IConfiguration> mOverrideConfiguration;
    AutoPtr<IResources> mResources;
};

} // namespace View
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_VIEW_CONTEXTTHEMEWRAPPER_H__
