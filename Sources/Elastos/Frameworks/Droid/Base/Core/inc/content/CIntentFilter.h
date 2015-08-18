
#ifndef __ELASTOS_DROID_CONTENT_CINTENTFILTER_H__
#define __ELASTOS_DROID_CONTENT_CINTENTFILTER_H__

#include "ext/frameworkext.h"
#include "_Elastos_Droid_Content_CIntentFilter.h"
#include "content/IntentFilter.h"

using Elastos::Droid::Net::IUri;
using Elastos::Droid::Os::IPatternMatcher;
using Elastos::Droid::Utility::IPrinter;
using Org::Xmlpull::V1::IXmlSerializer;
using Org::Xmlpull::V1::IXmlPullParser;

namespace Elastos {
namespace Droid {
namespace Content {

CarClass(CIntentFilter), public IntentFilter
{
public:
    /**
     * Modify priority of this filter.  The default priority is 0. Positive
     * values will be before the default, lower values will be after it.
     * Applications must use a value that is larger than
     * {@link #SYSTEM_LOW_PRIORITY} and smaller than
     * {@link #SYSTEM_HIGH_PRIORITY} .
     *
     * @param priority The new priority value.
     *
     * @see #getPriority
     * @see #SYSTEM_LOW_PRIORITY
     * @see #SYSTEM_HIGH_PRIORITY
     */
    CARAPI SetPriority(
        /* [in] */ Int32 priority);

    /**
     * Return the priority of this filter.
     *
     * @return The priority of the filter.
     *
     * @see #setPriority
     */
    CARAPI GetPriority(
        /* [out] */ Int32* priority);

    /**
     * Add a new Intent action to match against.  If any actions are included
     * in the filter, then an Intent's action must be one of those values for
     * it to match.  If no actions are included, the Intent action is ignored.
     *
     * @param action Name of the action to match, i.e. Intent.ACTION_VIEW.
     */
    CARAPI AddAction(
        /* [in] */ const String& action);

    /**
     * Return the number of actions in the filter.
     */
    CARAPI CountActions(
        /* [out] */ Int32* count);

    /**
     * Return an action in the filter.
     */
    CARAPI GetAction(
        /* [in] */ Int32 index,
        /* [out] */ String* action);

    /**
     * Is the given action included in the filter?  Note that if the filter
     * does not include any actions, false will <em>always</em> be returned.
     *
     * @param action The action to look for.
     *
     * @return True if the action is explicitly mentioned in the filter.
     */
    CARAPI HasAction(
        /* [in] */ const String& action,
        /* [out] */ Boolean* hasAction);

    /**
     * Match this filter against an Intent's action.  If the filter does not
     * specify any actions, the match will always fail.
     *
     * @param action The desired action to look for.
     *
     * @return True if the action is listed in the filter or the filter does
     *         not specify any actions.
     */
    CARAPI MatchAction(
        /* [in] */ const String& action,
        /* [out] */ Boolean* isMatched);

    /**
     * Return an iterator over the filter's actions.  If there are no actions,
     * returns null.
     * replace method: Iterator<String> actionsIterator()
     */
    CARAPI GetActions(
        /* [out, callee] */ ArrayOf<String>** actions);

    /**
     * Add a new Intent data type to match against.  If any types are
     * included in the filter, then an Intent's data must be <em>either</em>
     * one of these types <em>or</em> a matching scheme.  If no data types
     * are included, then an Intent will only match if it specifies no data.
     *
     * <p><em>Note: MIME type matching in the Android framework is
     * case-sensitive, unlike formal RFC MIME types.  As a result,
     * you should always write your MIME types with lower case letters,
     * and any MIME types you receive from outside of Android should be
     * converted to lower case before supplying them here.</em></p>
     *
     * <p>Throws {@link MalformedMimeTypeException} if the given MIME type is
     * not syntactically correct.
     *
     * @param type Name of the data type to match, i.e. "vnd.android.cursor.dir/person".
     *
     * @see #matchData
     */
    CARAPI AddDataType(
        /* [in] */ const String& type);

    /**
     * Is the given data type included in the filter?  Note that if the filter
     * does not include any type, false will <em>always</em> be returned.
     *
     * @param type The data type to look for.
     *
     * @return True if the type is explicitly mentioned in the filter.
     */
    CARAPI HasDataType(
        /* [in] */ const String& type,
        /* [out] */ Boolean* hasDataType);

    /**
     * Return the number of data types in the filter.
     */
    CARAPI CountDataTypes(
        /* [out] */ Int32* Count);

    /**
     * Return a data type in the filter.
     */
    CARAPI GetDataType(
        /* [in] */ Int32 index,
        /* [out] */ String* type);

    /**
     * Return an iterator over the filter's data types.
     * replace method: Iterator<String> typesIterator()
     */
    CARAPI GetTypes(
        /* [out, callee] */ ArrayOf<String>** types);

    /**
     * Add a new Intent data scheme to match against.  If any schemes are
     * included in the filter, then an Intent's data must be <em>either</em>
     * one of these schemes <em>or</em> a matching data type.  If no schemes
     * are included, then an Intent will match only if it includes no data.
     *
     * <p><em>Note: scheme matching in the Android framework is
     * case-sensitive, unlike formal RFC schemes.  As a result,
     * you should always write your schemes with lower case letters,
     * and any schemes you receive from outside of Android should be
     * converted to lower case before supplying them here.</em></p>
     *
     * @param scheme Name of the scheme to match, i.e. "http".
     *
     * @see #matchData
     */
    CARAPI AddDataScheme(
        /* [in] */ const String& scheme);

    /**
     * Return the number of data schemes in the filter.
     */
    CARAPI CountDataSchemes(
        /* [out] */ Int32* count);

    /**
     * Return a data scheme in the filter.
     */
    CARAPI GetDataScheme(
        /* [in] */ Int32 index,
        /* [out] */ String* scheme);

    /**
     * Is the given data scheme included in the filter?  Note that if the
     * filter does not include any scheme, false will <em>always</em> be
     * returned.
     *
     * @param scheme The data scheme to look for.
     *
     * @return True if the scheme is explicitly mentioned in the filter.
     */
    CARAPI HasDataScheme(
        /* [in] */ const String& scheme,
        /* [out] */ Boolean* hasDataScheme);

    /**
     * Return an iterator over the filter's data schemes.
     * replace method: Iterator<String> schemesIterator()
     */
    CARAPI GetSchemes(
        /* [out, callee] */ ArrayOf<String>** schemes);

    /**
     * Add a new Intent data authority to match against.  The filter must
     * include one or more schemes (via {@link #addDataScheme}) for the
     * authority to be considered.  If any authorities are
     * included in the filter, then an Intent's data must match one of
     * them.  If no authorities are included, then only the scheme must match.
     *
     * <p><em>Note: host name in the Android framework is
     * case-sensitive, unlike formal RFC host names.  As a result,
     * you should always write your host names with lower case letters,
     * and any host names you receive from outside of Android should be
     * converted to lower case before supplying them here.</em></p>
     *
     * @param host The host part of the authority to match.  May start with a
     *             single '*' to wildcard the front of the host name.
     * @param port Optional port part of the authority to match.  If null, any
     *             port is allowed.
     *
     * @see #matchData
     * @see #addDataScheme
     */
    CARAPI AddDataAuthority(
        /* [in] */ const String& host,
        /* [in] */ const String& port);

    /**
     * Return the number of data authorities in the filter.
     */
    CARAPI CountDataAuthorities(
        /* [out] */ Int32* count);

    /**
     * Return a data authority in the filter.
     */
    CARAPI GetDataAuthority(
        /* [in] */ Int32 index,
        /* [out] */ IIntentFilterAuthorityEntry** authority);

    /**
     * Is the given data authority included in the filter?  Note that if the
     * filter does not include any authorities, false will <em>always</em> be
     * returned.
     *
     * @param data The data whose authority is being looked for.
     *
     * @return Returns true if the data string matches an authority listed in the
     *         filter.
     */
    CARAPI HasDataAuthority(
        /* [in] */ IUri* Data,
        /* [out] */ Boolean* hasDataAuthority);

    /**
     * Return an iterator over the filter's data authorities.
     * replace method: Iterator<AuthorityEntry> authoritiesIterator()
     */
    CARAPI GetAuthorities(
        /* [out, callee] */ ArrayOf<IIntentFilterAuthorityEntry *>** authorities);

    /**
     * Add a new Intent data path to match against.  The filter must
     * include one or more schemes (via {@link #addDataScheme}) <em>and</em>
     * one or more authorities (via {@link #addDataAuthority}) for the
     * path to be considered.  If any paths are
     * included in the filter, then an Intent's data must match one of
     * them.  If no paths are included, then only the scheme/authority must
     * match.
     *
     * <p>The path given here can either be a literal that must directly
     * match or match against a prefix, or it can be a simple globbing pattern.
     * If the latter, you can use '*' anywhere in the pattern to match zero
     * or more instances of the previous character, '.' as a wildcard to match
     * any character, and '\' to escape the next character.
     *
     * @param path Either a raw string that must exactly match the file
     * path, or a simple pattern, depending on <var>type</var>.
     * @param type Determines how <var>path</var> will be compared to
     * determine a match: either {@link PatternMatcher#PATTERN_LITERAL},
     * {@link PatternMatcher#PATTERN_PREFIX}, or
     * {@link PatternMatcher#PATTERN_SIMPLE_GLOB}.
     *
     * @see #matchData
     * @see #addDataScheme
     * @see #addDataAuthority
     */
    CARAPI AddDataPath(
        /* [in] */ const String& path,
        /* [in] */ Int32 type);

    /**
     * Return the number of data paths in the filter.
     */
    CARAPI CountDataPaths(
        /* [out] */ Int32* count);

    /**
     * Return a data path in the filter.
     */
    CARAPI GetDataPath(
        /* [in] */ Int32 index,
        /* [out] */ IPatternMatcher** path);

    /**
     * Is the given data path included in the filter?  Note that if the
     * filter does not include any paths, false will <em>always</em> be
     * returned.
     *
     * @param data The data path to look for.  This is without the scheme
     *             prefix.
     *
     * @return True if the data string matches a path listed in the
     *         filter.
     */
    CARAPI HasDataPath(
        /* [in] */ const String& data,
        /* [out] */ Boolean* hasDataPath);

    /**
     * Return an iterator over the filter's data paths.
     * replace method: Iterator<PatternMatcher> pathsIterator()
     */
    CARAPI GetPaths(
        /* [out, callee] */ ArrayOf<IPatternMatcher *>** paths);

    /**
     * Match this intent filter against the given Intent data.  This ignores
     * the data scheme -- unlike {@link #matchData}, the authority will match
     * regardless of whether there is a matching scheme.
     *
     * @param data The data whose authority is being looked for.
     *
     * @return Returns either {@link #MATCH_CATEGORY_HOST},
     * {@link #MATCH_CATEGORY_PORT}, {@link #NO_MATCH_DATA}.
     */
    CARAPI MatchDataAuthority(
        /* [in] */ IUri* data,
        /* [out] */ Int32* result);

    /**
     * Match this filter against an Intent's data (type, scheme and path). If
     * the filter does not specify any types and does not specify any
     * schemes/paths, the match will only succeed if the intent does not
     * also specify a type or data.
     *
     * <p>Be aware that to match against an authority, you must also specify a base
     * scheme the authority is in.  To match against a data path, both a scheme
     * and authority must be specified.  If the filter does not specify any
     * types or schemes that it matches against, it is considered to be empty
     * (any authority or data path given is ignored, as if it were empty as
     * well).
     *
     * <p><em>Note: MIME type, Uri scheme, and host name matching in the
     * Android framework is case-sensitive, unlike the formal RFC definitions.
     * As a result, you should always write these elements with lower case letters,
     * and normalize any MIME types or Uris you receive from
     * outside of Android to ensure these elements are lower case before
     * supplying them here.</em></p>
     *
     * @param type The desired data type to look for, as returned by
     *             Intent.resolveType().
     * @param scheme The desired data scheme to look for, as returned by
     *               Intent.getScheme().
     * @param data The full data string to match against, as supplied in
     *             Intent.data.
     *
     * @return Returns either a valid match constant (a combination of
     * {@link #MATCH_CATEGORY_MASK} and {@link #MATCH_ADJUSTMENT_MASK}),
     * or one of the error codes {@link #NO_MATCH_TYPE} if the type didn't match
     * or {@link #NO_MATCH_DATA} if the scheme/path didn't match.
     *
     * @see #match
     */
    CARAPI MatchData(
        /* [in] */ const String& type,
        /* [in] */ const String& scheme,
        /* [in] */ IUri* data,
        /* [out] */ Int32* result);

    CARAPI AddCategory(
        /* [in] */ const String& category);

    /**
     * Return the number of categories in the filter.
     */
    CARAPI CountCategories(
        /* [out] */ Int32* count);

    /**
     * Return a category in the filter.
     */
    CARAPI GetCategory(
        /* [in] */ Int32 index,
        /* [out] */ String* category);

    CARAPI HasCategory(
        /* [in] */ const String& category,
        /* [out] */ Boolean* hasCategory);

    /**
     * Return an iterator over the filter's categories.
     *
     * @return Iterator if this filter has categories or {@code null} if none.
     * replace method: Iterator<String> categoriesIterator()
     */
    CARAPI GetCategories(
        /* [out, callee] */ ArrayOf<String>** categories);

    /**
     * Match this filter against an Intent's categories.  Each category in
     * the Intent must be specified by the filter; if any are not in the
     * filter, the match fails.
     *
     * @param categories The categories included in the intent, as returned by
     *                   Intent.getCategories().
     *
     * @return If all categories match (success), null; else the name of the
     *         first category that didn't match.
     */
    CARAPI MatchCategories(
        /* [in] */ ArrayOf<String>* categories,
        /* [out] */ String* result);

    /**
     * Test whether this filter matches the given <var>intent</var>.
     *
     * @param intent The Intent to compare against.
     * @param resolve If true, the intent's type will be resolved by calling
     *                Intent.resolveType(); otherwise a simple match against
     *                Intent.type will be performed.
     * @param logTag Tag to use in debugging messages.
     *
     * @return Returns either a valid match constant (a combination of
     * {@link #MATCH_CATEGORY_MASK} and {@link #MATCH_ADJUSTMENT_MASK}),
     * or one of the error codes {@link #NO_MATCH_TYPE} if the type didn't match,
     * {@link #NO_MATCH_DATA} if the scheme/path didn't match,
     * {@link #NO_MATCH_ACTION if the action didn't match, or
     * {@link #NO_MATCH_CATEGORY} if one or more categories didn't match.
     *
     * @return How well the filter matches.  Negative if it doesn't match,
     *         zero or positive positive value if it does with a higher
     *         value representing a better match.
     *
     * @see #match(String, String, String, android.net.Uri , Set, String)
     */
    CARAPI Match(
        /* [in] */ IContentResolver* resolver,
        /* [in] */ IIntent* intent,
        /* [in] */ Boolean resolve,
        /* [in] */ const String& logTag,
        /* [out] */ Int32* result);

    /**
     * Test whether this filter matches the given intent data.  A match is
     * only successful if the actions and categories in the Intent match
     * against the filter, as described in {@link IntentFilter}; in that case,
     * the match result returned will be as per {@link #matchData}.
     *
     * @param action The intent action to match against (Intent.getAction).
     * @param type The intent type to match against (Intent.resolveType()).
     * @param scheme The data scheme to match against (Intent.getScheme()).
     * @param data The data URI to match against (Intent.getData()).
     * @param categories The categories to match against
     *                   (Intent.getCategories()).
     * @param logTag Tag to use in debugging messages.
     *
     * @return Returns either a valid match constant (a combination of
     * {@link #MATCH_CATEGORY_MASK} and {@link #MATCH_ADJUSTMENT_MASK}),
     * or one of the error codes {@link #NO_MATCH_TYPE} if the type didn't match,
     * {@link #NO_MATCH_DATA} if the scheme/path didn't match,
     * {@link #NO_MATCH_ACTION if the action didn't match, or
     * {@link #NO_MATCH_CATEGORY} if one or more categories didn't match.
     *
     * @see #matchData
     * @see Intent#getAction
     * @see Intent#resolveType
     * @see Intent#getScheme
     * @see Intent#getData
     * @see Intent#getCategories
     */
    CARAPI Match(
        /* [in] */ const String& action,
        /* [in] */ const String& type,
        /* [in] */ const String& scheme,
        /* [in] */ IUri* data,
        /* [in] */ ArrayOf<String>* categories,
        /* [in] */ const String& logTag,
        /* [out] */ Int32* result);

    /**
     * Write the contents of the IntentFilter as an XML stream.
     */
    CARAPI WriteToXml(
        /* [in] */ IXmlSerializer* serializer);

    CARAPI ReadFromXml(
        /* [in] */ IXmlPullParser* parser);

    CARAPI Dump(
        /* [in] */ IPrinter* du,
        /* [in] */ const String& prefix);

    CARAPI HasPartialTypes(
        /* [out] */ Boolean* has);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ const String& action);

    CARAPI constructor(
        /* [in] */ const String& action,
        /* [in] */ const String& dataType);

    CARAPI constructor(
        /* [in] */ IIntentFilter* other);
};

} // namespace Content
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_CONTENT_CINTENTFILTER_H__
