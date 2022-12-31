const article = document.querySelector("article");

if (article) {
    const text = article.textContent;
    const wordMatchRegExp = /[^\s]+/g; // Regular expression
    const words = text.matchAll(wordMatchRegExp);
    // matchAll returns an iterator, so we need to convert it to an array
    const wordCount = [...words].length;
    const readingTime = Math.round(wordCount / 200);
    const badge = document.createElement("p");
    // use the same style as the article
    badge.classList.add("color-secondary-text", "type--caption");
    badge.textContent = `⏱️ ${readingTime} min read`;

    // Support for API reference docs
    const heading = article.querySelector("h1");

    // Support for article docs with date
    const date = article.querySelector("time")?.parentNode;
    
    (date ?? heading).insertAdjacentElement("afterend", badge);

}